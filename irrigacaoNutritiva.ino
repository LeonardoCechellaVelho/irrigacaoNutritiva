#include <Fuzzy.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define SERVO 0
#define ID_PH 1
#define ID_UMIDADE 2

/* LCD */
const int rs = 12, en = 11, d4 = 6, d5 = 4, d6 = 9, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/* Variáveis Globais */
int ph, umidade, irrigacaoOutput, irrigAnt;
Servo s;
byte quadrado[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

Fuzzy *fuzzy = new Fuzzy();

/* PH (Input) */
FuzzySet *phLow = new FuzzySet(0, 0, 50, 55);
FuzzySet *phMed = new FuzzySet(45, 57, 60, 65);
FuzzySet *phHigh = new FuzzySet(60, 65, 140, 140);

/* Umidade (Input) */
FuzzySet *umLow = new FuzzySet(0, 100, 750, 800);
FuzzySet *umMed = new FuzzySet(740, 820, 875, 900);
FuzzySet *umHigh = new FuzzySet(865, 910, 1000, 1000);

/* Irrigação Nutritiva (Output) */
FuzzySet *minima = new FuzzySet(0, 0, 15, 35);
FuzzySet *media = new FuzzySet(40, 50, 60, 80);
FuzzySet *maxima = new FuzzySet(70, 80, 100, 100);

/* Define irrigação nutritiva no minimo */
FuzzyRuleConsequent* irrigacaoMinima() {
  FuzzyRuleConsequent *entaoIrrigacaoMinima = new FuzzyRuleConsequent();
  entaoIrrigacaoMinima->addOutput(minima);
  return entaoIrrigacaoMinima;
}

/* Define irrigação nutritiva no médio */
FuzzyRuleConsequent* irrigacaoMedia() {
  FuzzyRuleConsequent *entaoIrrigacaoMedia = new FuzzyRuleConsequent();
  entaoIrrigacaoMedia->addOutput(media);
  return entaoIrrigacaoMedia;
}

/* Define irrigação nutritiva no máximo */
FuzzyRuleConsequent* irrigacaoMaxima() {
  FuzzyRuleConsequent *entaoIrrigacaoMaxima = new FuzzyRuleConsequent();
  entaoIrrigacaoMaxima->addOutput(maxima);
  return entaoIrrigacaoMaxima;
}

/* Regra AND para comparar valores */
FuzzyRuleAntecedent* antecedentWithAND(FuzzySet *value1, FuzzySet *value2) {
  FuzzyRuleAntecedent *ifWithAND = new FuzzyRuleAntecedent();
  ifWithAND->joinWithAND(value1, value2);
  return ifWithAND;
}

void setup()
{
  /* Definir tamanho do LCD "20x4" */
  lcd.begin(20, 4);
  lcd.createChar(0, quadrado);

  s.attach(SERVO);
  s.write(0);

  Serial.begin(9600);
  delay(2000);

  /* Setup fuzzy */
  inicializaInputsOutputs();
  criaRegras();
}

void inicializaInputsOutputs() {
  /* Cria input de pH */
  FuzzyInput *ph = new FuzzyInput(ID_PH);
  ph->addFuzzySet(phLow);
  ph->addFuzzySet(phMed);
  ph->addFuzzySet(phHigh);
  fuzzy->addFuzzyInput(ph);

  /* Cria input de Umidade */
  FuzzyInput *umidade = new FuzzyInput(ID_UMIDADE);
  umidade->addFuzzySet(umLow);
  umidade->addFuzzySet(umMed);
  umidade->addFuzzySet(umHigh);
  fuzzy->addFuzzyInput(umidade);

  /* Cria output de Irrigacao */
  FuzzyOutput *irrigacao = new FuzzyOutput(1);
  irrigacao->addFuzzySet(minima);
  irrigacao->addFuzzySet(media);
  irrigacao->addFuzzySet(maxima);
  fuzzy->addFuzzyOutput(irrigacao);
}

void criaRegras() {
  /* Regras de funcionamento */
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, antecedentWithAND(phLow, umLow), irrigacaoMaxima());
  fuzzy->addFuzzyRule(fuzzyRule1);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, antecedentWithAND(phLow, umMed), irrigacaoMedia());
  fuzzy->addFuzzyRule(fuzzyRule2);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, antecedentWithAND(phLow, umHigh), irrigacaoMinima());
  fuzzy->addFuzzyRule(fuzzyRule3);
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, antecedentWithAND(phMed, umLow), irrigacaoMedia());
  fuzzy->addFuzzyRule(fuzzyRule4);
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, antecedentWithAND(phMed, umMed), irrigacaoMedia());
  fuzzy->addFuzzyRule(fuzzyRule5);
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, antecedentWithAND(phMed, umHigh), irrigacaoMinima());
  fuzzy->addFuzzyRule(fuzzyRule6);
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, antecedentWithAND(phHigh, umLow), irrigacaoMinima());
  fuzzy->addFuzzyRule(fuzzyRule7);
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, antecedentWithAND(phHigh, umMed), irrigacaoMinima());
  fuzzy->addFuzzyRule(fuzzyRule8);
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, antecedentWithAND(phHigh, umHigh), irrigacaoMinima());
  fuzzy->addFuzzyRule(fuzzyRule9);
}

void telaLCD() {
  /* Lógica para o LCD */
  lcd.clear();
  float phf = ph;
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.setCursor(9, 0);
  lcd.print(umidade);
  lcd.setCursor(13, 0);
  lcd.print("mm");
  lcd.setCursor(0, 1);
  lcd.print("pH: ");
  lcd.setCursor(4, 1);
  lcd.print(phf/10);
  lcd.setCursor(0, 3);
  lcd.print("Irrigacao:");
  for(int i = 0; i < irrigacaoOutput/8; i++) {
    lcd.setCursor(i+10, 3);
    lcd.write(byte(0));
  }
}

void leSensor() {
  /* Leitura de valores e map dos mesmos */
  ph = map(analogRead(A1), 0, 1023, -20 , 164); 
  umidade = map(analogRead(A2), 0, 1023, -120, 1131); 
}

void fuzzySetup() {
  /* Setup dos inputs Fuzzy */
  fuzzy->setInput(ID_PH, ph);
  fuzzy->setInput(ID_UMIDADE, umidade);

  fuzzy->fuzzify();
  
  irrigacaoOutput = fuzzy->defuzzify(1);
}

void iteracaoMotor() {
  /* Alterar velocidade do motor */
  if (irrigAnt != irrigacaoOutput) {
    s.write(irrigacaoOutput);
    irrigAnt = irrigacaoOutput;
  }
}

void loop()
{
  leSensor();
  fuzzySetup();
  iteracaoMotor();
  telaLCD();
  
  delay(2000);
}
