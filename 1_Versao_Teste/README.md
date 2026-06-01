#  Prova de Conceito: Validação de Hardware

Esta pasta contém a versão inicial de testes do nosso projeto de controle de carga DC. 

O objetivo exclusivo deste código é realizar a **validação física das ligações** na protoboard (Módulo Relé, Bluetooth HC-05 e Cooler) utilizando as ferramentas de abstração da STMicroelectronics para agilizar o processo.

## ⚠️ Nota Importante de Engenharia
**Este não é o código final do projeto.** Para esta etapa de prototipagem rápida, utilizamos o **STM32CubeMX** e a biblioteca **HAL (Hardware Abstraction Layer)**. Isso nos permitiu testar o chaveamento do relé e a comunicação serial rapidamente sem precisar mapear os registradores do zero. 

A versão oficial, otimizada e escrita puramente em Bare-Metal (manipulação de registradores de baixo nível), encontra-se na pasta principal do repositório.

##  O Que Este Código Faz?

Este firmware foca em duas validações principais utilizando as funções de alto nível da HAL:
1. **Teste de Atuador (Blink do Relé):** Utiliza as funções `HAL_GPIO_WritePin()` e `HAL_Delay()` para atracar e soltar o relé em intervalos regulares (ex: a cada 2 segundos), garantindo que o circuito de potência e o cooler estão funcionando corretamente.
2. **Teste de Recepção (Opcional nesta etapa):** Inicializa a porta serial (USART1 a 9600 bps) via `MX_USART1_UART_Init()` para garantir que os pinos de TX/RX do HC-05 estão cruzados corretamente com a Bluepill.

##  Esquemático de Ligação

Para garantir que a montagem física reflita exatamente o que foi projetado no software, o circuito foi previamente modelado no EasyEDA. A montagem na protoboard deve seguir estritamente o diagrama lógico abaixo:

*(Arraste a sua imagem do EasyEDA para cá quando estiver editando no GitHub)*
![Esquemático de Teste na Protoboard](link_da_sua_imagem_aqui.png)

##  Pinagem de Teste (Resumo)

Caso precise de uma referência rápida para os cabos (jumpers), siga a tabela abaixo:

| Componente | Pino do Módulo | Pino STM32 (Bluepill) |
| :--- | :--- | :--- |
| **Relé** | IN (Sinal) | `PA5` |
| **HC-05** | TX | `PA10` |
| **HC-05** | RX | `PA9` |
| **ST-Link**| SWDIO / SWCLK | `PA13` / `PA14` |

##  Como Executar o Teste

1. Importe esta pasta específica (`1_Versao_Teste_PoC`) para dentro do seu **STM32CubeIDE**.
2. Compile o código e grave na placa usando o ST-Link V2.
3. Observe o comportamento físico do Módulo Relé (o "click" e o LED indicador) e o giro do Cooler.
4. Após confirmar que o hardware está 100% funcional, você pode prosseguir para a gravação da versão final em Bare-Metal!
