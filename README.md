#  Controle de Carga DC via Bluetooth com STM32

Este repositório contém o firmware, os códigos de teste e a documentação do projeto de Sistemas Embarcados desenvolvido para a disciplina de Microcontroladores na Universidade Federal do Ceará (UFC) - Campus Quixadá.

O objetivo do projeto é controlar o acionamento de um motor DC  remotamente através de comandos seriais via Bluetooth. O grande diferencial desta aplicação é a utilização de **interrupções de hardware puro (Bare-Metal)**, garantindo que o processador não desperdice tempo em laços de espera e fique livre para o processamento de outras tarefas simultâneas.

---

## Estrutura do Repositório (Metodologia)

O desenvolvimento seguiu a prática de **Prototipagem Rápida (PoC)**, dividindo o fluxo de trabalho em duas etapas claras:

* **`1_Versao_Teste/`**: Nossa Prova de Conceito. Esta versão inicial utiliza o STM32CubeMX e a biblioteca **HAL (Hardware Abstraction Layer)**. Foi utilizada exclusivamente para validar rapidamente as ligações elétricas físicas na bancada antes da escrita do código de baixo nível.

* **`2_Projeto_Final/`**: Versão oficial e definitiva de entrega. A dependência da biblioteca HAL foi removida. O acionamento, o temporizador (SysTick) e a leitura serial operam 100% via **Bare-Metal**, manipulando diretamente os registradores para atingir a máxima performance exigida pela arquitetura ARM Cortex-M.


```
2_Projeto_Final/
├── Core/
├── Drivers/
├── Relatorio/
│   └── Relatorio_Final.pdf
├── Documentos/
│   ├── Projeto_esquematico.pdf
│   └── Projeto_esquematico.png
└── README.md
```

### Documentação do Projeto

Toda a documentação complementar encontra-se disponível na pasta `Documentos`:

*  **Relatório Técnico:** [Relatorio_Final.pdf](./2_Projeto_Final/Relatorio/Relatorio_Final.pdf)

*  **Esquemático Elétrico (PDF):** [Projeto_esquematico.pdf](./2_Projeto_Final/Esquematico/Projeto_esquematico.pdf)

*  **Imagem do Esquemático:** [Projeto_esquematico.png](./2_Projeto_Final/Esquematico/Projeto_esquematico.png)

---

##  Pré-requisitos de Software

Para compilar, modificar ou gravar este projeto na placa, o ambiente de desenvolvimento requer:
* **STM32CubeIDE:** Ambiente de desenvolvimento oficial da STMicroelectronics.
* **Drivers ST-Link V2:** Para o reconhecimento do gravador USB pelo sistema operacional.

---

##  Plataforma e Componentes de Hardware

O hardware foi projetado para garantir o isolamento entre o circuito de controle (baixa potência) e o circuito de atuação (alta potência):

* **Microcontrolador:** Placa de Desenvolvimento STM32F103C8T6 (Bluepill - ARM Cortex-M3)
* **Comunicação:** Módulo Bluetooth HC-05 (Configurado para *Baud rate: 9600 bps*)
* **Atuador de Potência:** Módulo Relé Eletromecânico 1 Canal (5V)
* **Carga DC:** Cooler
* **Ferramenta de Gravação:** ST-Link V2

---

##  Arquitetura de Software e Funcionamento

A lógica de controle foi desenhada para ser estritamente **não-bloqueante** e orientada a eventos. 

1. O celular, através de um terminal Bluetooth, transmite os caracteres ASCII `'Y'` (Yes/Ligar) ou `'N'` (No/Desligar).
2. O laço principal do microcontrolador (`while(1)`) permanece completamente vazio, mantendo a CPU em estado de repouso ou livre para rotinas futuras.
3. Ao receber um dado via ar, o módulo HC-05 aciona o pino RX da STM32, o que dispara instantaneamente a interrupção de hardware **`RXNEIE`** (RX Not Empty Interrupt Enable).
4. A rotina de interrupção altera o registrador `GPIOA->BSRR` em **um único ciclo de máquina**.

---

##  Guia de Configuração do Celular

Adcionar após finalização da aplicação

---

## Demonstração

### Sistema Montado

![Montagem](imagens/montagem.jpg)

### Funcionamento

![Funcionamento](imagens/funcionamento.gif)

---

##  Referências Documentais

Para garantir o rigor técnico do código em Bare-Metal, as configurações basearam-se nas seguintes documentações oficiais da fabricante:
* [Reference Manual RM0008 (STMicroelectronics)](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf): Mapeamento dos registradores de Clock (RCC), GPIO (CRL/CRH, BSRR) e USART (CR1, DR).
* Datasheet Técnico do Módulo Bluetooth HC-05.
* Datasheet do Relé Eletromecânico Songle SRD-05VDC-SL-C.

---

##  Equipe Desenvolvedora
Desenvolvido com dedicação por:
* [Ana Alicy Ribeiro](https://github.com/AlicyRibeiro)
* [Ana Amélia](https://github.com/ameliasants)
* [Cícero Rodrigues](https://github.com/Icxxz)
