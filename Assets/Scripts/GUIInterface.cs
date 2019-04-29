using System;
using System.IO.Pipes;
using System.Threading;
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;

public class GUIInterface {

     static string time;
     static string Pname;
     static string Oname;
     static string Pscore;
     static string Oscore;
     static string ScreenMessage;
     static List<string> Rack=new List<string>();
     static List<List<string>> Letter=new List<List<string>>();
     static void SetRack(string rack){
         Rack.Clear();
         foreach (char letter in rack)
         {  if (letter!='0') {
           Rack.Add(letter.ToString()); 
         } 
         }
       }
     static void SetBoard(string playedword,int row,int column,string direction){
          Letter.Clear();
           foreach (char letter in playedword){ 
            if (letter!='0') {
            List<string> addedletter=new List<string>();
            addedletter.Add(letter.ToString());
            addedletter.Add(row.ToString());
            addedletter.Add(column.ToString());
            Letter.Add(addedletter);
           
            if (direction=="0")  //horizontal
              column++;
            else if (direction=="1")  //vertical
            row--;
          }
          }
       }
       public static List<string> Messages=new List<string>(); 
        static GUIInterface(){
            Pscore="0";
            Oscore="0";
            Pname="player1";
            Oname="player2";
        }

        public static void InterpretMessage(string message){
          string [] parameters=message.Split(',');
            if (parameters[0]!="-1"){
               time=parameters[1];
            }
          if (parameters[0]=="0"){
              Pname=parameters[2];
              Oname=parameters[3];       
              ScreenMessage="Connected";
          }
          else if (parameters[0]=="1"){
            ScreenMessage="Play";
            Pscore=parameters[2];
            Oscore=parameters[3];
            SetRack(parameters[4]);
            SetBoard(parameters[5],int.Parse(parameters[6]),int.Parse(parameters[7]),parameters[8]);  
          }
          else if (parameters[0]=="2"){
               Pscore=parameters[2];
              Oscore=parameters[3];
              ScreenMessage="Exchange";
              SetRack(parameters[4]);
          }
          else if (parameters[0]=="3"){
              Pscore=parameters[2];
              Oscore=parameters[3];
              // display pass
              ScreenMessage="Pass";
             
          }
          else if (parameters[0]=="4"){
              //challenge
              //display challenge
               Pscore=parameters[2];
               Oscore=parameters[3];
               ScreenMessage="Challenge";
          }
            else if (parameters[0]=="5"){
              //challenge
              //display challenge
               Pscore=parameters[2];
               Oscore=parameters[3];
               ScreenMessage="Start";
               SetRack(parameters[4]);
          }
           else if (parameters[0]=="-1"){
            // terminate connnection
             ScreenMessage="Connection terminated";
          }
          
    
        }

        public static string Gettime(){
            return time;
        }
        public static string GetPname(){
            return Pname;
        }
        public static string GetOname(){
           return Oname;
        }
        public static List<string> GetRack(){
            return Rack;
        } 
        public static List<List<string>> GetLetters(){
            return Letter;
        }
        public static string GetPscore(){
            return Pscore;
        }
        public static string GetOscore(){
            return Oscore;
        }
        public static string GetScreenMessage(){
            return ScreenMessage;
        }
 
    }

      