
# ⚡ Controle de Carga DC via Bluetooth com STM32 

Este repositório contém o código-fonte e a documentação do projeto final desenvolvido para a disciplina de Sistemas Microcontrolados da Universidade Federal do Ceará (UFC) - Campus Quixadá.

O objetivo do projeto é realizar o acionamento de uma carga DC (Cooler) através de comandos seriais via Bluetooth (HC-05), utilizando um microcontrolador STM32. Para garantir o máximo de eficiência, o projeto utiliza uma **arquitetura híbrida**, mesclando a biblioteca HAL com programação *Bare-Metal* (manipulação direta de registradores).

---

##  Equipe
* Ana Alicy
* Ana Amélia
* Cícero Rodrigues

---

##  Estrutura do Repositório

O projeto foi construído em etapas metodológicas. Aqui você encontrará as seguintes pastas:

*  **`Projeto_Final/`**: Contém o código oficial e finalizado. Utiliza a interrupção serial (RXNEIE) e manipulação de registradores (Bare-Metal) para o controle da carga de forma otimizada.
*  **`Teste_Rele_HC05/`**: Contém os códigos de validação de hardware (Prova de Conceito). Usado para testar o chaveamento do módulo relé e a comunicação básica do HC-05 com o Cooler antes da integração final.

---

##  Componentes de Hardware
* **Microcontrolador:** Placa de desenvolvimento STM32F103C8T6 (Bluepill)
* **Comunicação:** Módulo Bluetooth HC-05
* **Atuador:** Módulo Relé 1 Canal (5V)
* **Carga:** Cooler DC
* **Gravador:** ST-Link V2

---

