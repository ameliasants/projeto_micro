# ⚡ Controle de Carga DC via Bluetooth com STM32

Este repositório contém o firmware, os códigos de teste e a documentação do projeto de Sistemas Embarcados desenvolvido para a disciplina de Sistemas Microcontrolados na Universidade Federal do Ceará (UFC) - Campus Quixadá.

O objetivo do projeto é controlar o acionamento de um motor DC (Cooler) remotamente através de comandos seriais via Bluetooth. O grande diferencial desta aplicação é a utilização de **interrupções de hardware puro (Bare-Metal)**, garantindo que o processador não desperdice tempo em laços de espera e fique livre para o processamento de outras tarefas simultâneas.

---

## 📑 Índice
1. [Estrutura do Repositório](#-estrutura-do-repositório-metodologia)
2. [Pré-requisitos de Software](#-pré-requisitos-de-software)
3. [Plataforma e Hardware](#️-plataforma-e-componentes-de-hardware)
4. [Arquitetura de Software](#-arquitetura-de-software-e-funcionamento)
5. [Mapeamento de Pinos (Pinout)](#-diagrama-de-ligações-e-pinout)
6. [Guia de Teste via Celular](#-guia-de-configuração-do-celular)
7. [Referências Documentais](#-referências-documentais)

---

## 📁 Estrutura do Repositório (Metodologia)

O desenvolvimento seguiu a prática de **Prototipagem Rápida (PoC)**, dividindo o fluxo de trabalho em duas etapas claras:

* 📂 **`1_Versao_Teste/`**: Nossa Prova de Conceito. Esta versão inicial utiliza o STM32CubeMX e a biblioteca **HAL (Hardware Abstraction Layer)**. Foi utilizada exclusivamente para validar rapidamente as ligações elétricas físicas na bancada antes da escrita do código de baixo nível.
* 📂 **`2_Projeto_Final/`**: Versão oficial e definitiva de entrega. A dependência da biblioteca HAL foi removida. O acionamento, o temporizador (SysTick) e a leitura serial operam 100% via **Bare-Metal**, manipulando diretamente os registradores para atingir a máxima performance exigida pela arquitetura ARM Cortex-M.

---

## 💻 Pré-requisitos de Software

Para compilar, modificar ou gravar este projeto na placa, o ambiente de desenvolvimento requer:
* **STM32CubeIDE:** Ambiente de desenvolvimento oficial da STMicroelectronics.
* **Drivers ST-Link V2:** Para o reconhecimento do gravador USB pelo sistema operacional.

---

## 🛠️ Plataforma e Componentes de Hardware

O hardware foi projetado para garantir o isolamento entre o circuito de controle (baixa potência) e o circuito de atuação (alta potência):

* **Microcontrolador:** Placa de Desenvolvimento STM32F103C8T6 (Bluepill - ARM Cortex-M3)
* **Comunicação:** Módulo Bluetooth HC-05 (Configurado para *Baud rate: 9600 bps*)
* **Atuador de Potência:** Módulo Relé Eletromecânico 1 Canal (5V)
* **Carga DC:** Cooler
* **Ferramenta de Gravação:** ST-Link V2

---

## 🧠 Arquitetura de Software e Funcionamento

A lógica de controle foi desenhada para ser estritamente **não-bloqueante** e orientada a eventos. 

1. O celular, através de um terminal Bluetooth, transmite os caracteres ASCII `'Y'` (Yes/Ligar) ou `'N'` (No/Desligar).
2. O laço principal do microcontrolador (`while(1)`) permanece completamente vazio, mantendo a CPU em estado de repouso ou livre para rotinas futuras.
3. Ao receber um dado via ar, o módulo HC-05 aciona o pino RX da STM32, o que dispara instantaneamente a interrupção de hardware **`RXNEIE`** (RX Not Empty Interrupt Enable).
4. A rotina de interrupção altera o registrador `GPIOA->BSRR` em **um único ciclo de máquina**.

> **Exemplo do Acionamento Bare-Metal (Trecho do Código):**
> ```c
> ```

---

## 🔌 Diagrama de Ligações e Pinout

Para garantir a replicação exata do projeto, o circuito foi documentado esquematicamente.

*(Insira a imagem do esquemático exportado do EasyEDA aqui)*
![Esquemático de Ligações do Sistema](link_para_sua_imagem_aqui.png)

### Tabela de Mapeamento

| Componente | Pino do Módulo | Pino STM32 (Bluepill) | Função / Registrador Alvo |
| :--- | :--- | :--- | :--- |
| **HC-05** | `TX` | `PA10` | RX (Recepção Serial via USART1) |
| **HC-05** | `RX` | `PA9` | TX (Transmissão Serial via USART1) |
| **Relé** | `IN` (Sinal) | `PA5` | Saída Digital Push-Pull (`GPIOA->BSRR`) |
| **ST-Link**| `SWDIO` | `PA13` | Interface de Debug |
| **ST-Link**| `SWCLK` | `PA14` | Clock de Debug |

---

## 📱 Guia de Configuração do Celular

Para enviar os comandos remotos para a placa, utilize um aplicativo de terminal serial, como o **Serial Bluetooth Terminal** (Google Play).

1. Alimente o sistema (o LED do HC-05 piscará rapidamente, aguardando pareamento).
2. No menu Bluetooth do celular, pareie com o dispositivo **HC-05** (Senha padrão: `1234` ou `0000`).
3. Abra o aplicativo *Serial Bluetooth Terminal*, acesse a aba `Devices`, selecione o HC-05 e conecte.
4. No terminal, envie os seguintes comandos:
   * **`Y`** (Maiúsculo): A interrupção processa o dado, atracando o relé e ligando o cooler.
   * **`N`** (Maiúsculo): A interrupção corta o sinal, soltando o contato e desligando o motor.

---

## 📚 Referências Documentais

Para garantir o rigor técnico do código em Bare-Metal, as configurações basearam-se nas seguintes documentações oficiais da fabricante:
* [Reference Manual RM0008 (STMicroelectronics)](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf): Mapeamento dos registradores de Clock (RCC), GPIO (CRL/CRH, BSRR) e USART (CR1, DR).
* Datasheet Técnico do Módulo Bluetooth HC-05.
* Datasheet do Relé Eletromecânico Songle SRD-05VDC-SL-C.

---

## 👥 Equipe Desenvolvedora
Desenvolvido com dedicação por:
* Ana Alicy Ribeiro dos Santos
* Ana Amélia
* Cícero Rodrigues
