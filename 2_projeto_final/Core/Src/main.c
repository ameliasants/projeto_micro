#include "stm32f1xx.h"
#include <stdio.h>
#include <string.h>

// ==========================================================
// CONFIGURAÇÃO DO DISJUNTOR VIRTUAL
// ==========================================================
#define LIMITE_CORRENTE_MA 3000 // Desarma se passar de 3 Ampères (3000 mA)

void delay_ms(uint32_t ms) {
    SysTick->LOAD = 8000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    for (uint32_t i = 0; i < ms; i++) {
        while ((SysTick->CTRL & (1 << 16)) == 0);
    }
    SysTick->CTRL = 0;
}

void UART1_SendString(char* str) {
    while (*str) {
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = (*str & 0xFF);
        str++;
    }
}

uint32_t calcular_raiz(uint32_t n) {
    if (n == 0) return 0;
    uint32_t x = n;
    uint32_t y = 1;
    while (x > y) {
        x = (x + y) / 2;
        y = n / x;
    }
    return x;
}

void USART1_IRQHandler(void) {
    if (USART1->SR & USART_SR_RXNE) {
        uint8_t dado = USART1->DR;
        if (dado == '1') GPIOA->BSRR = (1 << 5);
        else if (dado == '0') GPIOA->BSRR = (1 << 21);
    }
}

int main(void) {
    // 1. Inicialização
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN | RCC_APB2ENR_USART1EN;
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV2;

    GPIOA->CRL &= ~((0xF << 0) | (0xF << 20));
    GPIOA->CRH &= ~((0xF << 4) | (0xF << 8));

    GPIOA->CRL |= (0x3 << 20); // PA5 (Relé)
    GPIOA->CRH |= (0xB << 4);  // PA9 (TX)
    GPIOA->CRH |= (0x4 << 8);  // PA10 (RX)

    USART1->BRR = 0x341;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART1_IRQn);

    // 2. Calibração Interna do ADC
    ADC1->CR2 |= ADC_CR2_ADON;
    delay_ms(1);
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    char mensagem[60]; // Aumentado um pouquinho por segurança

    // ==========================================================
    // 3. NOVA CALIBRAÇÃO DINÂMICA DO SENSOR (AUTO-ZERO)
    // ==========================================================
    uint32_t soma_zero = 0;
    for(int i = 0; i < 5000; i++) {
        ADC1->CR2 |= ADC_CR2_ADON;
        while (!(ADC1->SR & ADC_SR_EOC));
        soma_zero += ADC1->DR;
    }
    int32_t zero_adc = soma_zero / 5000;
    // ==========================================================

    // ==========================================================
    // VARIÁVEIS DO MEDIDOR DE ENERGIA
    // ==========================================================
    uint32_t tensao_rede = 220;  // Tensão da tomada
    uint64_t energia_Ws = 0;     // Cofre que acumula os Watts-segundo

    // LAÇO INFINITO
    while (1) {
        uint64_t soma_quadrados = 0;
        uint32_t amostras = 1500;

        for (uint32_t i = 0; i < amostras; i++) {
            ADC1->CR2 |= ADC_CR2_ADON;
            while (!(ADC1->SR & ADC_SR_EOC));
            int32_t leitura = ADC1->DR;

            int32_t diferenca = leitura - zero_adc;
            soma_quadrados += (diferenca * diferenca);
        }

        uint32_t media_quadrados = soma_quadrados / amostras;
        uint32_t rms_adc = calcular_raiz(media_quadrados);

        // Filtro de Zona Morta
        if (rms_adc < 100) {
            rms_adc = 0;
        }

        // 1. Converte para Corrente
        int32_t tensao_mV = (rms_adc * 3300) / 4095;
        int32_t corrente_mA = (tensao_mV * 1000) / 185;

        // ==========================================================
        // 2. DISJUNTOR VIRTUAL (SEGURANÇA CONTRA SOBRECARGA)
        // ==========================================================
        if (corrente_mA > LIMITE_CORRENTE_MA) {
            GPIOA->BSRR = (1 << 21); // Desliga o relé fisicamente (joga o PA5 para 0)
            UART1_SendString("\r\n!!! ALERTA DE SOBRECARGA !!! Relé desligado.\r\n");
            delay_ms(500); // Pausa rápida para o Bluetooth enviar a mensagem inteira
        }

        // 3. Calcula Potência (W) e Acumula Energia (Ws)
        uint32_t potencia_W = (tensao_rede * corrente_mA) / 1000;
        energia_Ws += potencia_W;

        // 4. Converte para kWh separando a parte inteira da decimal
        uint32_t kWh_inteiro = energia_Ws / 3600000;
        uint32_t kWh_fracao = (uint32_t)(( (uint64_t)(energia_Ws % 3600000) * 10000ULL ) / 3600000ULL);

        // 5. Imprime APENAS o Consumo formatado
        sprintf(mensagem, "Consumo: %ld mA | %lu.%04lu kWh\r\n", corrente_mA, kWh_inteiro, kWh_fracao);
        UART1_SendString(mensagem);

        delay_ms(1000);
    }
}
