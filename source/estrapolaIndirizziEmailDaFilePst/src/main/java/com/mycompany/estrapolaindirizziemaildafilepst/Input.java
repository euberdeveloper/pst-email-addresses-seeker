package com.mycompany.estrapolaindirizziemaildafilepst;

import java.io.BufferedReader;
import java.io.InputStreamReader;

//Classe di base input per ricevere input da prompt con tutte le sue sottoclassi
class Input 
{
 protected static final BufferedReader FLUSSO_INPUT = new BufferedReader(new InputStreamReader(System.in));

 public static final byte byteReturn()
 {
  try
  {
   return Byte.parseByte(FLUSSO_INPUT.readLine());
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + "in input");
   System.exit(0);
   return -1;
  }
 }
 
 public static final short shortReturn()
 {
  try
  {
   return Short.parseShort(FLUSSO_INPUT.readLine());
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + "in input");
   System.exit(0);
   return -1;
  }
 }
 
 public static final int intReturn()
 {
  try
  {
   return Integer.parseInt(FLUSSO_INPUT.readLine());
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + "in input");
   System.exit(0);
   return -1;
  }
 }
 
 public static final long longReturn()
 {
  try
  {
   return Long.parseLong(FLUSSO_INPUT.readLine());
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + "in input");
   System.exit(0);
   return -1;
  }
 }
 
 public static final float floatReturn()
 {
  try
  {
   return Float.parseFloat(FLUSSO_INPUT.readLine());
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + "in input");
   System.exit(0);
   return -1;
  }
 }
 
 public static final double doubleReturn()
 {
  try
  {
   return Double.parseDouble(FLUSSO_INPUT.readLine());
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + "in input");
   System.exit(0);
   return -1;
  }
 }
 
 public static final char charReturn()
 {
  try 
  {
   String temp = FLUSSO_INPUT.readLine();
   if(temp.length() > 1)
   {
    System.out.println("Errore: l'input deve essere un char, non uno String");
    System.exit(0);
    return '\0';
   }  
   return temp.charAt(0);
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + " in input");
   System.exit(0);
   return '\0';
  }
 }
 
 public static final String stringReturn()
 {
  try
  {
   return FLUSSO_INPUT.readLine();
  }
  catch(Exception e)
  {
   System.out.println("Errore: " + e + " in input");
   System.exit(0);
   return "";
  }       
 }
}
