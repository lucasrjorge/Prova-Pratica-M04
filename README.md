## Modos de Operação
### Modo Claro
O modo claro é ativado quando o LDR detecta uma luminosidade maior do que 60, após isso acontecer o led verde fica ligado por 3 segundos, o amarelo por 2 e o vermelho por 5. Além de leds o sistema também conta com um botão, que se pressionado em conjunto com o led vermelho, após 1 segundo o sinal vermelho fecha e o verde abre, alternativamente, se o botão for pressionado 3 vezes enquanto o led vermelho está ligado, o sistema fará uma requisição HTTP (um GET especificamente).

### Modo Escuro
Caso o LDR detecte uma luminosidade inferior, o led amarelo passará a ligar e desligar em um intervalo de 1 segundo.

### Conexão WiFi
A conexão WiFi não pode ser realizada no tempo delimitado para a prova por problemas com o Wokwi, que não estava aguentando a demanda de requisições em seu servidor e bloqueou o sinal de Wifi para muitos alunos (a maioria deles) durante a prova. Por conta disso o código não conseguiu rodar por completo e fiquei impossibilitado de colocar prints do sistema funcionando
