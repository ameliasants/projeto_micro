#  Controle de Carga DC via Bluetooth com STM32 (Abordagem Híbrida)

Este repositório contém o código-fonte e a documentação do projeto final desenvolvido para a disciplina de Sistemas Microcontrolados da Universidade Federal do Ceará (UFC) - Campus Quixadá.

O objetivo do projeto é realizar o acionamento de uma carga DC (Cooler) através de comandos seriais via Bluetooth (HC-05), utilizando um microcontrolador STM32. Para garantir o máximo de eficiência, o projeto utiliza uma **arquitetura híbrida**, mesclando a biblioteca HAL com programação *Bare-Metal* (manipulação direta de registradores).

##  Equipe
* Ana Alicy
* Ana Amélia
* Cícero Rodrigues

##  Componentes de Hardware
* **Microcontrolador:** Placa de desenvolvimento STM32F103C8T6 (Bluepill)
* **Comunicação:** Módulo Bluetooth HC-05
* **Atuador:** Módulo Relé 1 Canal (5V)
* **Carga:** Cooler DC
* **Gravador:** ST-Link V2

##  Arquitetura de Software (HAL + Bare-Metal)
Para aliar a rapidez de desenvolvimento com a performance de execução em tempo real, o firmware foi dividido em duas camadas:

1. **Inicialização (HAL):** A burocracia de configuração de clocks (SysClock), pinagem (GPIO) e a taxa de transmissão serial (USART a 9600 bps) foi feita utilizando a biblioteca padrão HAL gerada pelo STM32CubeMX.
2. **Execução e Interrupção (Bare-Metal):** O laço principal (`while(1)`) é mantido 100% livre. A recepção de dados via Bluetooth "arma" uma interrupção de hardware nativa (`RXNEIE`). Quando os caracteres `'Y'` (Liga) ou `'N'` (Desliga) são recebidos, a rotina `USART1_IRQHandler` manipula o registrador `GPIOA->BSRR` diretamente para acionar o Relé em um único ciclo de clock, sem atrasos de software.

##  Esquemático de Ligação
*(Dica: Adicione aqui a imagem do circuito que você exportou do EasyEDA)*
![Esquemático do Projeto](link_da_sua_imagem_aqui.png)

* **HC-05:** TX no `PA10` (RX) | RX no `PA9` (TX)
* **Relé:** Pino de Sinal IN conectado ao `PA5`
* **ST-Link:** SWDIO no `PA13` | SWCLK no `PA14`

##  Como Compilar e Executar
1. Clone este repositório: `git clone https://github.com/AlicyRibeiro/seu-repositorio.git`
2. Abra a pasta do projeto no **STM32CubeIDE**.
3. Compile o projeto (ícone do martelo ou `Ctrl+B`).
4. Conecte o ST-Link V2 ao computador e à Bluepill.
5. Clique em **Debug** ou **Run** para gravar o firmware no microcontrolador.
6. Pelo aplicativo de celular (ex: *Serial Bluetooth Terminal*), pareie com o módulo HC-05 e envie as letras `Y` para ligar e `N` para desligar.
