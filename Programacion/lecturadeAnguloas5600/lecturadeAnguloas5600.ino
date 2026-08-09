#include <Wire.h>

// Dirección I2C de fábrica del AS5600
#define AS5600_ADDRESS 0x36 

// Registros de memoria donde se guarda la posición (Angle)
#define RAW_ANGLE_MSB 0x0C  // Byte más significativo
#define RAW_ANGLE_LSB 0x0D  // Byte menos significativo

void setup() {
  // Iniciar comunicación serial para ver los datos en pantalla
  Serial.begin(115200);
  
  // Iniciar el bus I2C
  Wire.begin(); 
  
  // Opcional: Aumentar la velocidad del I2C a 400kHz para lecturas más rápidas en control PID
  Wire.setClock(400000); 
}

void loop() {
  // 1. Obtener la lectura cruda del sensor
  int raw_angle = leerAnguloAS5600();
  
  // 2. Convertir el valor crudo a grados
  // El AS5600 es de 12 bits, su valor máximo es 4095 (4096 pasos)
  float grados = (raw_angle / 4096.0) * 360.0;

  // 3. Imprimir en el Monitor Serie
  Serial.print("Valor crudo: ");
  Serial.print(raw_angle);
  Serial.print("  |  Posición: ");
  Serial.print(grados);
  Serial.println(" grados");

  delay(10); // Pequeña pausa de 10ms
}

// Función dedicada para leer los registros por I2C
int leerAnguloAS5600() {
  int angle = 0;
  
  // Apuntar al registro donde empieza el dato (MSB)
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(RAW_ANGLE_MSB);
  Wire.endTransmission();

  // Solicitar 2 bytes de información al sensor
  Wire.requestFrom(AS5600_ADDRESS, 2);
  
  // Verificar si recibimos los 2 bytes esperados
  if (Wire.available() <= 2) {
    byte msb = Wire.read(); // Leer el primer byte
    byte lsb = Wire.read(); // Leer el segundo byte
    
    // Unir los dos bytes.
    // Desplazamos el MSB 8 bits a la izquierda y aplicamos un OR lógico con el LSB
    angle = (msb << 8) | lsb;
  }
  
  return angle;
}
