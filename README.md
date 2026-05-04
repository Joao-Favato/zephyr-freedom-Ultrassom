# Zephyr-Freedom-Ultrassom

## 1. Implementação do Trigger
Para começar a implementação do sensor ultrassom, comecei pela criação do trigger utilizando PWM. A fim de conseguir realizar isso, foram enfrentadas algumas dificuldades, como a definição da frequência correta dos sinais e a conversão dos ticks para os intervalos de tempo adequados. Depois de configurar adequadamente, testei a saída da porta A13 com um osciloscópio para me certificar do funcionamento. Durante a realização dos testes, foi utilizado um pulso de aproximadamente 1 ms com o objetivo de facilitar a visualização. 

<img width="394" height="700" alt="TesteDoTrigger" src="evidencias\TesteTrigger.gif" />

## 2. Configuração do Input Capture
Para testar a saida no serial monitor foi configurado o pino pta12 para printar o momento de detecção do pulso de subida, por meio do input capture. Para validar este codigo nós estabelecemos uma conexao por meio de um fio do tipo macho-macho entre o pino pta12 e o pino pwm ptd3.

<img width="394" height="700" alt="TesteDoTrigger" src="evidencias\Gravação de Tela 2026-05-04 153810.gif" />

## 3. Medição da largura do pulso do PWM
Verificamos a frequência do CLOCK para podermos calcular corretamente a largura e o tempo do pulso PWM, calculados que o tempo equivalente a 10 microsegundos é de 485 ticks.

<img width="394" height="700" alt="TesteDoTrigger" src="evidencias\Gravação de Tela 2026-05-04 165234 (1).gif" />




