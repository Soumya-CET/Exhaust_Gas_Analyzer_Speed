/************************Hardware Related Macros************************************/
const int calibrationLed = 13;                      //when the calibration start , LED pin 13 will light up , off when finish calibrating
//const int MQ_PIN=A0;                              //define which analog input channel you are going to use
// A0 for mq2 analog value read from sensor
int RL_VALUE=5;                                     //define the load resistance on the board, in kilo ohms
float mq2_RO_CLEAN_AIR_FACTOR=9.83;                 //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                    //which is derived from the chart in datasheet
/***********************Software Related Macros************************************/
int CALIBARAION_SAMPLE_TIMES=50;                    //define how many samples you are going to take in the calibration phase
int CALIBRATION_SAMPLE_INTERVAL=500;                //define the time interal(in milisecond) between each samples in the
                                                    //cablibration phase
int READ_SAMPLE_INTERVAL=50;                        //define how many samples you are going to take in normal operation
int READ_SAMPLE_TIMES=5;                            //define the time interal(in milisecond) between each samples in 
                                                    //normal operation
/**********************Application Related Macros**********************************/
#define         mq2_ch4             0   
#define         mq2_co              1   
#define         GAS_SMOKE           2    
/*****************************Globals***********************************************/
float           CH4Curve[3]  =  {2.3,0.47,-0.37};   //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve. 
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.47), point2: (lg10000, -0.15) 
float           COCurve[3]  =  {2.3,0.72,-0.34};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
float           SmokeCurve[3] ={2.3,0.53,-0.44};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     
float           mq2_ro           =  10;             //Ro is initialized to 10 kilo ohms

void setup()
{ 
  
  Serial.begin(9600);
  pinMode(calibrationLed,OUTPUT);
  digitalWrite(calibrationLed,HIGH);
  Serial.print("Calibrating...");                        //LCD display
  mq2_ro = MQCalibration(A0,mq2_RO_CLEAN_AIR_FACTOR);    //Calibrating the sensor. Please make sure the sensor is in clean air         
  digitalWrite(calibrationLed,LOW);              
  
  Serial.println("done!");                               //LCD display
  Serial.println("mq2_ro= ");
  Serial.println(mq2_ro);
  Serial.println("kohm");
  delay(3000);
}
 
void loop()
{  
  long iPPM_CH4 = 0;
  long iPPM_CO = 0;
  long iPPM_Smoke = 0;

  iPPM_CH4 = MQGetGasPercentage(MQRead(A0)/mq2_ro,mq2_ch4);
  iPPM_CO = MQGetGasPercentage(MQRead(A0)/mq2_ro,mq2_co);
  iPPM_Smoke = MQGetGasPercentage(MQRead(A0)/mq2_ro,GAS_SMOKE);
  
   
   Serial.println("Concentration of gas ");
   
  
   Serial.println("LPG: ");
   Serial.println(iPPM_CH4);
   Serial.println(" ppm");   
  
   
   Serial.println("CO: ");
   Serial.println(iPPM_CO);
   Serial.println(" ppm");    

   
   Serial.println("Smoke: ");
   Serial.println(iPPM_Smoke);
   Serial.println(" ppm");  

   delay(200);
  
}
 
/****************** MQResistanceCalculation ****************************************
Input:   raw_adc - raw value read from adc, which represents the voltage
Output:  the calculated sensor resistance
************************************************************************************/ 
float MQResistanceCalculation(int raw_adc)
{
  //Serial.println(((float)RL_VALUE*(1023-raw_adc)/raw_adc));
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}
 
/***************************** MQCalibration ****************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
************************************************************************************/ 
float MQCalibration(int mq_pin, float ro)
{
  int i;
  float val=0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
  val = val/ro;                        //divided by  yields the Ro                                        
  return val;                                                      //according to the chart in the datasheet 

}
 
/*****************************  MQRead *********************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
************************************************************************************/ 
float MQRead(int mq_pin)
{
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
}
 
/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
************************************************************************************/ 
long MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == mq2_ch4 ) {
     return MQGetPercentage(rs_ro_ratio,CH4Curve);
  } else if ( gas_id == mq2_co ) {
     return MQGetPercentage(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_SMOKE ) {
     return MQGetPercentage(rs_ro_ratio,SmokeCurve);
  }    
 
  return 0;
}
 
/*****************************  MQGetPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
********************************************************************/ 
long  MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
