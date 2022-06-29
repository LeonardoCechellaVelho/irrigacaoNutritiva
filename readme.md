# Irrigação Nutritiva

Projeto utilizado para simular a aplicação de uma irrigação nutritiva automática em uma horta. Com o auxilio do software PICSIMLab para simular a placa eletrônica Arduino UNO.

## Rodando o projeto

- Na IDE do Arduino, ir na aba Sketch > Exportar binário compilado (Ctrl + Alt + S);
- Instalar a versão 0.8.10 do [PICSIMLab](https://sourceforge.net/projects/picsim/);
- Alterar para a placa Arduino Uno na aba "Board"; 
- Carregar o arquivo workspace.pzw ao PICSIMLab pela aba File > Load Workspace;
- Após isso ir em File > Load Hex, e ler o arquivo binário compilado anteriormente;
- Para visualizar os componentes pelo PICSIMLab acesse na aba Modules > Spare parts.

## Placa PICSIMLab

![PlacaPIC](https://github.com/LeonardoCechellaVelho/irrigacaoNutritiva/blob/master/assets/img/previewPlaca.png?raw=true)

## Metodologia

### Entradas:
- Sensor de pH;
- Sensor de umidade do solo.

### Saídas:
- Motor (Bomba d'agua com a solução nutritiva).

### Variável de entrada pH:
- Universo: 0 a 14;
- Conjunto Fuzzy: Baixo, Médio e Alto;
- Função de pertinência:

![FuncaoPertinenciaPH](https://github.com/LeonardoCechellaVelho/irrigacaoNutritiva/blob/master/assets/img/pertinenciaPH.png?raw=true)

### Variável de entrada umidade:
- Universo: 0mm a 1000mm de umidade;
- Conjunto Fuzzy: Baixa, Mediana e Alta;
- Função de pertinência:

![FuncaoPertinenciaUmidade](https://github.com/LeonardoCechellaVelho/irrigacaoNutritiva/blob/master/assets/img/pertinenciaUmidade.png?raw=true)

### Variável de entrada umidade:
- Universo: Quanto de umidade e solução nutritiva o solo necessita de 0 a 1;
- Conjunto Fuzzy: Baixa, Mediana e Alta;
- Função de pertinência:

![FuncaoPertinenciaIrrigacaoNutritiva](https://github.com/LeonardoCechellaVelho/irrigacaoNutritiva/blob/master/assets/img/pertinenciaIrrigacao.png?raw=true)

### Regras do sistema:
- Quando o sensor de PH do solo estiver perto de 5 ou 6,5 separar a solução nutritiva nas plantas. Ler umidade do solo antes de enviar solução nutritiva (Inferior a 900 mm). ([Fonte para pH ideal do solo](https://www.assimquefaz.com/horta-em-casa-o-ph-ideal-do-solo-para-cada-planta/)); 

- Quando o sensor de umidade do solo indicar que a umidade está inferior a 750mm irrigar a horta. ([Fonte para umidade ideal do solo](https://eventos.unipampa.edu.br/sciprot/files/2019/09/sciprot_2019_paper_131.pdf)).
