#include <iostream>
#include <fstream>
#include <string>
#include <clocale>

#include "List_nodes.h"
#include "Wstring_converter.h"

//Da notare che la libreria è stata fatta nel 2010 è il compilatore del momento era gcc 4.4 con std=c++0x ed il gcc5 e parzialmente supportato era 
//il gcc 4.4 senza std=c++0x. Con i nuovi compilatori potrebbe dare un segnale di errore perchè non si usa il std=c++0x, ma non perchè il compilatore
//è da aggiornare ma perchè usa un std più nuovo. Su può lasciare il messaggio o sopprimerlo definendo la costante SUPPRESS_CPLUSPLUS0X_MESSAGES
//fatta per i compilatori parzialmente supportati. La libreria dovrebbe in entrambi i casi funzionare. Ricordarsi di aggiungere i path delle librerie
//pstsdk e boost 1.42 nel path dell'include del compilatore in uso
#define SUPPRESS_CPLUSPLUS0X_MESSAGES
#include "pstsdk/pst.h"

using namespace std;
using namespace pstsdk;

int M=0;

/* FUNZIONI ACCESSORIE */
 
//Funzione che descrive il programma
void descrizioneProgramma();

/* FUNZIONI RIGUARDANTI LA LISTA */

//Funzione che inserisce l'elemento nella lista solo se non è già presente
template<class T_elem>
void Insert(const T_elem, List<T_elem>&);

/* FUNZIONI RIGUARDANTI IL RICONOSCIMENTO DELL'INDIRIZZO EMAIL */

//Funzione che controlla che il carattere sia possibile in un indirizzo email (funzione di servizio)
bool isEmailAddressCharacter(char);

//Funzione che controlla che il carattere sia possibile in un indirizzo email (funzione di servizio)
bool isEmailAddress(string);

//Funzione che prende gli indirizzi email da una stringa di testo e li mette nella lista del secondo parametro (funzione di servizio)
void takeEmailAddressFromText(string&, List<string>&);

//Funzione che elimina l'ultimo carattere di uno string
string popBack(string);

/* FUNZIONI RIGUARDANTI IL PST */

//Chiede in input il nome del file pst e lo ritorna come wstring
wstring openPstFile();

//Prende gli indirizzi dall'oggetto e dal testo del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromSubjectAndBody(const message&, List<string>&);

//Prende gli embedded messages(messaggi in allegato) dagli allegati del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeAddressFromAttachment(const attachment&, List<string>&);

//Prende gli embedded messages(messaggi in allegato) dagli allegati del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromEmbeddedMessages(const message&, List<string>&);

//Prende gli indirizzi dai destinatari del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeAddressFromRecipient(const recipient&, List<string>&);

//Prende gli indirizzi dai destinatari del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromRecipients(const message&, List<string>&);

//Prende gli indirizzi dal messaggio passato come primo parametro e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromMessage(const message&, List<string>&);

//Prende gli indirizzi dal pst passato come primo parametro accedendo ad ogni messaggio e li mette nel List passato come secondo parametro
void takeEmailAddressFromPst(const pst&, List<string>&);

/* FUNZIONI RIGUARDANTI IL CSV */

//Funzione che scrive nel csv dato come primo parametro le stringhe contenute nella lista data come secondo parametro (funzione di servizio)
void putListInCsv(ofstream& file, List<string>& indirizzi);

//Funzione che chiede in input il nome del file csv, lo apre in scrittura, scrive i dati presenti nella lista data come parametro e chiude il file
void writeCsvFile(List<string>& indirizzi);

int main() {
			//Informazioni sul programma
			descrizioneProgramma();
			//Dichiaro la lista di stringhe che conterrà gli indirizzi
			List<string> indirizzi;
			//Apro e ricavo il file pst
			pst pst_file(openPstFile());
			//Ricavo gli indirizzi e li metto nella lista
			takeEmailAddressFromPst(pst_file, indirizzi);
			//Creo il file csv contenente gli indirizzi
			writeCsvFile(indirizzi);
			system("pause");
			return 0;
		   }

/* FUNZIONI ACCESSORIE */

//Funzione che descrive il programma
void descrizioneProgramma()
{
 //Rendo possibili i caratteri locali (lettere accentate nel caso dell'Italia)
 setlocale(LC_ALL,"");
 //Scrivo la descrizione del programma
 cout<<"Questo programma legge gli indirizzi email contenuti in un file pst e li scrive uno sotto l'altro in un file csv.\n"
 	 <<"Gli indirizzi sono letti dal mittente, i destinatari CC, quelli scritti nell'oggetto e nel testo di ogni messaggio e quelli scritti in eventuali messaggi in allegato.\n"
 	 <<"Perchè il programma funzioni bisogna che il programma sia nella stessa cartella del file pst. Verrà chiesto il nome del file csv dove saranno scritti gli indirizzi\n";
 cout<<endl;
 return;
}
		   
/* FUNZIONI RIGUARDANTI LA LISTA */

//Funzione che inserisce l'elemento nella lista solo se non è già presente
template<class T_elem>
void Insert(const T_elem el, List<T_elem>& list)
{
 if(list.Find(el) == -1)
  list.Push_back(el);
 return;	
}		   
		   
/* FUNZIONI RIGUARDANTI IL RICONOSCIMENTO DELL'INDIRIZZO EMAIL */

//Funzione che controlla che il carattere sia possibile in un indirizzo email (funzione di servizio)
bool isEmailAddressCharacter(char ch)
{
 //In un indirizzo email sono permessi: lettere(a-z, A-Z), numeri(0-9), trattini(-,_), il punto(.)
 if(isalnum(ch) || ch=='-' || ch=='_' || ch=='.')
  return true;
 return false;
}

//Funzione che controlla che la parola sia un indirizzo email (funzione di servizio)
bool isEmailAddress(string word)
{
 bool chiocciola = false;
 bool punto = false;
 //Se ci sono degli spazi o la stringa è vuota ritorna false
 if(word.find(" ") != -1 || word.empty() == true)
 {
  return false;
 }
 //Controllo sulla sintassi
 for(int i=0; i<word.length(); i++)
 {
  if(chiocciola == false)
  {
  	if(i!=0 && word[i] == '@')
  	 chiocciola = true;
  	else if(isEmailAddressCharacter(word[i]) == false)
  	 return false;
  }
  else if(punto == false)
  {
   if(word[i-1]!='@' && word[i]=='.')
   	punto = true;
   else if(isEmailAddressCharacter(word[i]) == false)
  	 return false;
  }
  else
  {
   if(isEmailAddressCharacter(word[i]) == false)
  	 return false;
  }
 }
 if(chiocciola && punto)
  return true;
 else
  return false;
}

//Funzione che prende gli indirizzi email da una stringa di testo e li mette nella lista del secondo parametro (funzione di servizio)
void takeEmailAddressFromText(string& s, List<string>& indirizzi)
{
 //Dichiaro la variabile contatore e la stringa temporanea
 int i;
 string temp;
 //Sostituisco virgole, due punti, punti e virgola, \n, \t, punti esclamativi, punti di domanda, doppi apici, apici singoli, parentesi, maggiore o minore, uguale, più con spazi
 for(i=0; i<s.length(); i++)
  if(s[i]==',' || s[i]==':' || s[i]==';' || s[i]=='\n' || s[i]=='\t' || s[i]=='!' || s[i]=='?' || s[i]=='\"' || s[i]=='\'' || s[i]=='(' || s[i]==')' || s[i]=='<' || s[i]=='>' || s[i]=='=' || s[i]=='+' || s[i]=='[' || s[i]==']')
   s[i]=' ';
 //Eseguo il controllo e l'assegnamento
 for(i=0; i<s.length(); i++)
 {
  if(s[i] != ' ')
  {
   temp.push_back(s[i]);
   if(i == (s.length() - 1))
   {
   	if(isEmailAddress(temp))
   	{
   	 if(temp[temp.length() - 1] == '.') //se l'ultimo carattere è un punto lo elimino
   	  temp = popBack(temp);
   	 Insert(temp,indirizzi);
    }
   }
   else if(s[i+1] == ' ')
   {
   	if(isEmailAddress(temp))
   	{
   	 if(temp[temp.length() - 1] == '.') //se l'ultimo carattere è un punto lo elimino
   	  temp = popBack(temp);
   	 Insert(temp,indirizzi);
    }
   }
  }
  else
   temp.clear();
 }
 return;
}

//Funzione che elimina l'ultimo carattere di uno string
string popBack(string s)
{
 string temp;
 int i;
 for(i=0; i<s.length() - 1; i++)
  temp.push_back(s[i]);
 return temp;
}

/* FUNZIONI RIGUARDANTI IL PST */

//Chiede in input il nome del file pst e lo ritorna come wstring
wstring openPstFile()
{
 wstring nome_file;
 cout<<"Scrivi il nome del file pst (senza estensione)\n";
 getline(wcin, nome_file);
 nome_file += L".pst";
 return nome_file;
}

//Prende gli indirizzi dall'oggetto e dal testo del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromSubjectAndBody(const message& msg, List<string>& indirizzi)
{
 //Dichiaro le variabili contenenti l'oggetto ed il testo
 string oggetto, testo;
 //Ricavo l'oggetto e il testo se ci sono
 if(msg.has_subject())
  oggetto = wstring_to_string(msg.get_subject());
 if(msg.has_body())
  testo = wstring_to_string(msg.get_body());  
 //Prendo gli indirizzi email dall'oggetto e dal testo del messaggio
 takeEmailAddressFromText(oggetto, indirizzi);
 takeEmailAddressFromText(testo, indirizzi);
 return;
}

//Prende gli embedded messages(messaggi in allegato) dagli allegati del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeAddressFromAttachment(const attachment& attach, List<string>& indirizzi)
{
 if(attach.is_message())
 {
  message msg = attach.open_as_message();
  takeEmailAddressFromMessage(msg, indirizzi);
 }
 return;
}

//Prende gli embedded messages(messaggi in allegato) dagli allegati del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromEmbeddedMessages(const message& msg, List<string>& indirizzi)
{	
 //Faccio un'iterazione di tutti gli allegati
 if(msg.get_attachment_count() > 0)
 {
  for(message::attachment_iterator iter = msg.attachment_begin(); iter != msg.attachment_end(); ++iter)
  {
   takeAddressFromAttachment(*iter, indirizzi);
  }
 }
 return;
}

//Prende gli indirizzi dai destinatari del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeAddressFromRecipient(const recipient& r, List<string>& indirizzi)
{
 //Prendo il nome utente, l'account name e l'email address se ci sono
 string nome, account_name, indirizzo_email;
 nome = wstring_to_string(r.get_name());
 if(r.has_account_name()) //Se c'è l'account name
  account_name = wstring_to_string(r.get_account_name());
 if(r.has_email_address()) //Se c'è un indirizzo email
  indirizzo_email = wstring_to_string(r.get_email_address());
 //Prendo gli indirizzi email dai dati ricevuti
 takeEmailAddressFromText(nome, indirizzi);
 takeEmailAddressFromText(account_name, indirizzi);
 takeEmailAddressFromText(indirizzo_email, indirizzi);
 return;
}

//Prende gli indirizzi dai destinatari del messaggio e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromRecipients(const message& msg, List<string>& indirizzi)
{
 //Faccio un'iterazione di tutti i destinatari
 if(msg.get_recipient_count() > 0)
 {
  for(message::recipient_iterator iter = msg.recipient_begin(); iter != msg.recipient_end(); ++iter)
  {
   takeAddressFromRecipient(*iter, indirizzi);
  }
 }
 return;
}

//Prende gli indirizzi dal messaggio passato come primo parametro e li mette nel List passato come secondo parametro (funzione di servizio)
void takeEmailAddressFromMessage(const message& msg, List<string>& indirizzi)
{
 //Si estrapolano dall'oggetto e dal testo
 takeEmailAddressFromSubjectAndBody(msg, indirizzi);
 //Come allegati ci possono essere dei messaggi, si controllano anche quelli
 takeEmailAddressFromEmbeddedMessages(msg, indirizzi);
 //Si estrapolano dai destinatari
 takeEmailAddressFromRecipients(msg, indirizzi);
 return;
}

//Prende gli indirizzi dal pst passato come primo parametro accedendo ad ogni messaggio e li mette nel List passato come secondo parametro
void takeEmailAddressFromPst(const pst& file, List<string>& indirizzi)
{
 //Faccio un'iterazione di tutti i messaggi all'interno del file
 for(pst::message_iterator iter = file.message_begin(); iter != file.message_end(); ++iter)
 {
  takeEmailAddressFromMessage(*iter, indirizzi);
  M++;
 }
 return;
}

/* FUNZIONI RIGUARDANTI IL CSV */

//Funzione che scrive nel csv dato come primo parametro le stringhe contenute nella lista data come secondo parametro (funzione di servizio)
void putListInCsv(ofstream& file, List<string>& indirizzi)
{
 //Ordinamento alfabetico degli indirizzi contenuti nella lista
 indirizzi.Ascending();
 //Dichiarazione del contatore
 int i;
 //Immissione dei dati
 for(i=0; i<indirizzi.Size(); i++)
  file<<indirizzi.Value(i)<<endl;
 cout<<"Immessi "<<indirizzi.Size()<<" indirizzi nel file csv\n";
 return;
}

//Funzione che chiede in input il nome del file csv, lo apre in scrittura, scrive i dati presenti nella lista data come parametro e chiude il file
void writeCsvFile(List<string>& indirizzi)
{
 //Input del nome del file da creare/modificare
 string nome_file;
 cout<<"Scrivi il nome del file csv su cui scrivere gli indirizzi (senza estensione)\n";
 getline(cin, nome_file);
 nome_file+=".csv";
 //Apro il file csv
 ofstream file(nome_file.c_str());//Converto lo string in const char*
 if(!file)
 {
  cout<<"Errore nell'apertura del file csv\n";
  system("pause");
  exit(1);
 }
 //Scrivo le stringhe della lista nel csv
 putListInCsv(file,indirizzi);
 //Chiudo il file
 file.close();
 return;
}
