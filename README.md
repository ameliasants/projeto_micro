# ⚡ Controle de Carga DC via Bluetooth com STM32

Este repositório contém o firmware, os códigos de teste e a documentação do projeto de Sistemas Embarcados desenvolvido para a disciplina de Sistemas Microcontrolados na Universidade Federal do Ceará (UFC) - Campus Quixadá.

O objetivo do projeto é controlar o acionamento de um motor DC (Cooler) remotamente através de comandos seriais via Bluetooth. O grande diferencial desta aplicação é a utilização de **interrupções de hardware puro (Bare-Metal)**, garantindo que o processador não desperdice tempo em laços de espera e fique livre para o processamento de outras tarefas.

## 📁 Estrutura do Repositório (Metodologia)

O desenvolvimento seguiu a prática de Prototipagem Rápida, sendo dividido em duas etapas que se encontram em pastas separadas neste repositório:

* 📂 **`1_Versao_Teste/`**: Nossa Prova de Conceito (PoC). Esta versão inicial utiliza o STM32CubeMX e a biblioteca **HAL (Hardware Abstraction Layer)**. Foi utilizada exclusivamente para validar rapidamente as ligações elétricas (Protoboard, ST-Link, Relé e HC-05) antes da escrita do código de baixo nível.
* 📂 **`2_Projeto_Final/`**: Versão oficial e definitiva. A dependência da biblioteca HAL foi removida. O acionamento, o temporizador (SysTick) e a leitura serial operam 100% via **Bare-Metal**, manipulando diretamente os registradores (`BSRR`, `CR1`, `CRL`, etc.) para atingir a máxima performance e cumprir os requisitos técnicos da arquitetura ARM.

## 💻 Pré-requisitos de Software

Para compilar, modificar ou gravar este projeto na placa, você precisará ter instalado em sua máquina:
* **STM32CubeIDE:** Ambiente de desenvolvimento oficial da STMicroelectronics.
* **Drivers ST-Link:** Para que o computador reconheça o gravador USB (ST-Link V2).

## 🛠️ Plataforma e Componentes de Hardware

O sistema foi arquitetado em torno do núcleo ARM Cortex-M3. Foram utilizados os seguintes hardwares e ferramentas:

* **Microcontrolador:** Placa STM32F103C8T6 (Bluepill)
* **Comunicação:** Módulo Bluetooth HC-05 (Baud rate: 9600 bps)
* **Atuador de Potência:** Módulo Relé Eletromecânico de 1 Canal (5V)
* **Carga:** Cooler DC
* **Gravação e Debug:** ST-Link V2
* **Esquemático:** Diagrama lógico desenhado via EasyEDA

## 🧠 Arquitetura de Software e Funcionamento

A lógica foi desenhada para ser não-bloqueante e orientada a eventos:

1. O celular, através de um terminal Bluetooth, envia os caracteres ASCII `'Y'` (Yes/Ligar) ou `'N'` (No/Desligar).
2. O laço principal do microcontrolador (`while(1)`) permanece completamente vazio, mantendo a CPU em repouso.
3. Ao receber um dado via ar, o módulo HC-05 aciona o pino RX da STM32, o que dispara instantaneamente a interrupção de hardware **`RXNEIE`** (RX Not Empty Interrupt Enable).
4. A rotina de interrupção (`USART1_IRQHandler`) lê o caractere recebido e altera os bits de *Set* ou *Reset* do registrador `GPIOA->BSRR`, atracando ou soltando o relé em um único ciclo de máquina.

## 🔌 Diagrama de Ligações (Pinagem)

*(Dica: Insira a imagem do esquemático exportado do EasyEDA aqui)*
![Esquemático de Ligações do Sistema](link_para_sua_imagem_aqui.png)

* **HC-05:** Pino TX conectado ao `PA10` (RX) | Pino RX conectado ao `PA9` (TX)
* **Relé:** Pino de controle (IN) conectado ao `PA5`
* **ST-Link:** SWDIO no `PA13` | SWCLK no `PA14`

## 📱 Guia de Configuração do Celular (Passo a Passo)

Para enviar os comandos remotos para a placa, você precisará de um aplicativo de terminal serial. Recomendamos o **Serial Bluetooth Terminal** (disponível na Google Play).

1. Alimente a placa STM32 e o módulo HC-05 (o LED vermelho do módulo começará a piscar rápido, indicando modo de pareamento).
2. No celular, vá em *Configurações > Bluetooth* e pareie com o dispositivo **HC-05** (A senha padrão geralmente é `1234` ou `0000`).
3. Abra o aplicativo *Serial Bluetooth Terminal*, vá na aba `Devices`, selecione o HC-05 e clique no ícone de conectar.
4. No terminal de mensagens, digite os seguintes comandos de controle:
   * **`Y`** (Maiúsculo) e envie: O microcontrolador processa a interrupção, o relé atraca e o cooler liga.
   * **`N`** (Maiúsculo) e envie: O microcontrolador processa a interrupção, o relé solta o contato e o cooler desliga.

## 📚 Referências Documentais

Para o desenvolvimento em Bare-Metal e o correto mapeamento da arquitetura, consultamos as seguintes documentações técnicas oficiais:
* [Reference Manual RM0008 (STMicroelectronics)](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf): Utilizado como base principal para mapear os registradores de Clock (RCC), GPIO (CRL/CRH, BSRR) e USART (CR1, DR).
* Datasheet Técnico do Módulo Bluetooth HC-05.
* Datasheet do Relé Eletromecânico Songle SRD-05VDC-SL-C.

## 👥 Equipe Desenvolvedora

* Ana Alicy Ribeiro dos Santos
* Ana Amélia
* Cícero Rodrigues
