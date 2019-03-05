/**
 * @since 08/05/2016
 */

package com.mycompany.estrapolaindirizziemaildafilepst;

import com.opencsv.CSVReader;
import com.opencsv.CSVWriter;
import com.pff.PSTAttachment;
import com.pff.PSTContact;
import com.pff.PSTException;
import com.pff.PSTFile;
import com.pff.PSTFolder;
import com.pff.PSTMessage;
import com.pff.PSTObject;
import com.pff.PSTRecipient;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;

public class EstrapolaIndirizziEmailDaFilePst {
    
    //Lista di String che conterrà i vari indirizzi email
    private static ArrayList<String> emailAddresses = new ArrayList<>();
    
    //Stringa contenente il nome del file pst
    private static String pstFileName;
    
    //File pst
    private static PSTFile pstFile;
    
    //RootFolder del pst (Cartella Radice)
    private static PSTFolder rootFolder;
    
    //ArrayList contenente tutte le cartelle del pst
    private static ArrayList<PSTFolder> folders = new ArrayList<>();
    
    //Array di String contenente tutte le cartelle da analizzare
    private static String[] foldersToAnalize;
    
    //Stringa contenente il nome del file csv
    private static String csvFileName;
    
    //Indirizzo cartella pst e forse csv
    private static String pathUtil;
    
    //File csv
    private static CSVWriter csvFile; 
    
    //Stringa contenente il path del progetto(per la blacklist)
    private static String currentPath;
    
    //ArrayList di String contenente i domini da non salvare
    private static ArrayList<String> blacklist = new ArrayList<>();
    
    //METODO PRINCIPALE (Main del programma)
    public static void main(String[] args) throws PSTException, IOException {
        //Assegno il path della blacklist
        currentPath = System.getProperty("user.dir") + "/blacklist.csv";
        //Sistemo la blacklist
        getBlackList(currentPath, blacklist);
        //Assegno a pstFileName il nome del file pst ricevuto in input
        pstFileName = getPstFileName();
        //Apro il file pst
        pstFile = openPstFile(pstFileName);
        //Chiedo all'utente se vuole controllare tutte le cartelle o solo alcune
        String risposta;
        do
        {   
            System.out.println("Vuoi controllare tutto il file o solo alcune cartelle? (full/part)");
            risposta = Input.stringReturn();
            if(!(risposta.equals("full") || risposta.equals("part"))) {
                System.out.println("Devi scrivere o \"full\" o \"part\", ripeto la domanda");
            }
        }
        while(!(risposta.equals("full") || risposta.equals("part")));
        //Se risponde part gli chiedo le cartelle che vuole controllare e controllo solo quelle
        if(risposta.equals("part")) {
            //Scrivo un commento
            System.out.println("N.B: se viene selezionata la cartella madre vengono controllate anche tutte le sottocartelle");
            //Prendo tutte le cartelle e le mostro
            showAndGetFolders(pstFile, folders);
            //Prendo in input quelle da controllare
            foldersToAnalize = getFoldersNumber().split(",");
            //Prendo gli indirizzi email da esse
            getEmailAddressFromChosenFolders(folders, foldersToAnalize, emailAddresses);
        }
        //Altrimenti gli controllo tutto il file
        else {
            //Prendo gli indirizzi email
            getEmailAddressFromPstFile(pstFile, emailAddresses);
        }
        //Assegno a csvFileName il nome del file csv ricevuto in input
        csvFileName = getCsvFileName();
        //Apro il file csv
        csvFile = openCsvFile(csvFileName);
        //Scrivo gli indirizzi 
        putEmailAddressesInCsvFile(csvFile, emailAddresses);
        //Chiudo il file csv
        csvFile.close(); 
    }
    
    /* METODI RIGUARDANTI LA BLACKLIST (CSV) */
    
    //Metodo che legge il file CSV contenente la blacklist e li mette nella lista
    private static void getBlackList(String path, ArrayList<String> blacklist) throws IOException {   
        //Se trovo il file
        try 
        {
            //Apro il file csv
            CSVReader csvBlacklist = new CSVReader(new FileReader(path));
            //Aggiungo tutto nell'ArrayList<String> blacklist
            getDomini(csvBlacklist, blacklist);
            //Se la blacklist è vuota
            if(blacklist.isEmpty()) {
                //Chiedo se l'utente vuole creare la blacklist
                String temp;
                do
                {   
                    System.out.println("La tua blacklist è vuota, la creo? (si/no)");
                    temp = Input.stringReturn();
                    if(!(temp.equals("no") || temp.equals("si"))) {
                        System.out.println("Devi scrivere o \"si\" o \"no\", ripeto la domanda");
                    }
                }
                while(!(temp.equals("no") || temp.equals("si")));
                //Se risponde si, la creo
                if(temp.equals("si")) {
                    makeBlackList(path, blacklist);
                }
            }
            //Se non è vuota
            else {
                //Chiedo se l'utente vuole aggiungere altri domini
                String temp;
                do
                {   
                    System.out.println("Vuoi aggiungere altri domini? (si/no)");
                    temp = Input.stringReturn();
                    if(!(temp.equals("no") || temp.equals("si"))) {
                        System.out.println("Devi scrivere o \"si\" o \"no\", ripeto la domanda");
                    }
                }
                while(!(temp.equals("no") || temp.equals("si")));
                //Se risponde si aggiorno la blacklist
                if(temp.equals("si")) {
                    //Aggiungo domini al csv
                    reMakeBlackList(path, blacklist);
                }
            }

        } 
        //Se non trova il file avvisa
        catch (FileNotFoundException ex) 
        {
            //Chiedo se l'utente vuole creare la blacklist
            String temp;
            do
            {   
                System.out.println("Non esiste ancora una blacklist, la creo? (si/no)");
                temp = Input.stringReturn();
                if(!(temp.equals("no") || temp.equals("si"))) {
                    System.out.println("Devi scrivere o \"si\" o \"no\", ripeto la domanda");
                }
            }
            while(!(temp.equals("no") || temp.equals("si")));
            //Se risponde si creo la blacklist
            if(temp.equals("si")) {
                makeBlackList(path, blacklist);
            }
        } 
    }
    
    //Metodo che apre la all'utente i domini e poi li mette nella blacklist
    private static void reMakeBlackList(String path, ArrayList<String> blacklist) throws IOException {
        //Creo il file CSV della blaklist
        CSVWriter csvBlacklist = openCsvFile(path);
        //Aggiungo i domini che ci sono già nella blacklist
        for(String s : blacklist)
        {
            csvBlacklist.writeNext(new String[]{s});
        }
        //Faccio aggiungere all'utente i domini alla blacklist
        updateBlackList(csvBlacklist);
        //Aggiungo i domini nell'ArrayList<String>
        CSVReader csvReadBlackList;
        //Riapro il file CSV come lettura
        csvReadBlackList = new CSVReader(new FileReader(path));
        //Aggiungo i domini all'ArrayList e li stampo
        getDomini(csvReadBlackList, blacklist);
    }
    
    //Metodo che crea la blacklist e fa aggiungere all'utente i domini e poi li mette nella blacklist
    private static void makeBlackList(String path, ArrayList<String> blacklist) throws IOException {
        //Creo il file CSV della blaklist
        CSVWriter csvBlacklist = openCsvFile(path);
        //Faccio aggiungere all'utente i domini alla blacklist
        updateBlackList(csvBlacklist);
        //Aggiungo i domini nell'ArrayList<String>
        CSVReader csvReadBlackList = null;
        //Riapro il file CSV come lettura
        csvReadBlackList = new CSVReader(new FileReader(path));
        getDomini(csvReadBlackList, blacklist);
    }
    
    //Metodo che fa aggiungere all'utente i domini
    private static void updateBlackList(CSVWriter csvBlacklist) throws IOException {
        //In un ciclo, faccio aggiungere all'utente i domini
        String temp, dominio;
        do
        {
           System.out.println("Scrivi il dominio che vuoi aggiungere");
           dominio = Input.stringReturn();
           //Se è un dominio lo aggiungo, altrimenti glielo dico
           if(isDominio(dominio)) {
            csvBlacklist.writeNext(new String[]{dominio});
           }
           else {
               System.out.println("La parola data non ha la sintassi di un dominio");
           }
           //Chiedo se vuole aggiungere un altro dominio
           do
           {
            System.out.println("Vuoi aggiungere un altro dominio(si/no)?");
            temp = Input.stringReturn();
            if(!(temp.equals("no") || temp.equals("si"))) {
                System.out.println("Devi scrivere o \"si\" o \"no\", ripeto la domanda");
            }
           }
           while(!(temp.equals("no") || temp.equals("si")));
        }
        while(temp.equals("si"));
        //Chiudo il file CSV della blacklist
        csvBlacklist.close();
    }
    
    //Metodo che mette i domini scritti nel csv nella blacklist
    private static void getDomini(CSVReader csvBlacklist, ArrayList<String> blacklist) throws IOException {
        List temp = null;
        temp = csvBlacklist.readAll();
        //Aggiungo tutto nell'ArrayList<String> blacklist
        for(Object t : temp)
        {
            for(String s: (String[])t)
            {
                insertElement(s, blacklist);
            }
        }
        //Stampo la blacklist se non è vuota
        if(!blacklist.isEmpty()) {
            System.out.println("La tua blacklist è:");
            for(String s : blacklist)
            {
                System.out.println(s);
            }
        }
    }
    
    //Metodo che controlla se la parola data è un dominio
    private static boolean isDominio(String dominio) {
        String[] s = dominio.split(Pattern.quote(".")); //Uso Pattern.quote() perchè il punto è considerato un carattere speciale per split
        return (s.length == 2);
    }
    
    /* METODI RIGUARDANTI IL FILE PST */
    
    //Metodo per chiedere il nome del programma
    private static String getPstFileName() {
        //Chiedo di scrivere l'indirizzo della cartella contenente il file pst
        System.out.println("Scrivi l'indirizzo della cartella contenente il file pst");
        //Assegno alla stringa l'input ed aggiungo "\"
        String indirizzoCartella = Input.stringReturn() + "\\";
        pathUtil = indirizzoCartella;
        //Chiedo di scrivere il nome del file pst
        System.out.println("Scrivi il nome del file pst in input (senza estensione)");
        //Ricevo in input il nome del file pst, aggiungo l'indirizzo e l'estensione ".pst" e lo ritorno
        return indirizzoCartella + Input.stringReturn() + ".pst";
    }
   
    //Metodo che ritorna il file pst aperto e come parametro chiede l'indirizzo del file
    private static PSTFile openPstFile(String fileName) throws PSTException, IOException {
        return new PSTFile(fileName);
    }
    
    //Metodo che mostra tutte le cartelle numerate all'utente e le mette in un ArrayList di PSTFolder (Prima esecuzione su cartella madre)
    private static void showAndGetFolders(PSTFile pstFile, ArrayList<PSTFolder> folders) throws IOException, PSTException {
        //Dichiaro la PSTFolder che conterrà la rootFolder
        PSTFolder rootFolder;
        //Prendo la rootFolder
        rootFolder = pstFile.getRootFolder();
        //Aggiungo all'ArrayList la rootFolder
        folders.add(rootFolder);
        //Scrivo all'utente numero della cartella(folders.size()) e nome della cartella
        System.out.println(folders.size() - 1 + ")\tRootFolder/");
        //Se ha sottocartelle lo faccio anche per loro
        if(rootFolder.hasSubfolders()) {
            Vector<PSTFolder> childFolders;
            childFolders = rootFolder.getSubFolders();
            for(PSTFolder childFolder : childFolders) {
                showAndGetFolders(childFolder, "-----", folders);
            }
        }
        
    }
    
    //Metodo che mostra tutte le cartelle numerate all'utente e le mette in un ArrayList di PSTFolder (Esecuzione su cartelle figlie)
    private static void showAndGetFolders(PSTFolder subFolder, String parentFolder, ArrayList<PSTFolder> folders) throws PSTException, IOException {
        //Aggiungo all'ArrayList la sottocartella
        folders.add(subFolder);
        //Scrivo all'utente numero della cartella(folders.size()) e nome della cartella
        System.out.println(folders.size() - 1 + ")\t" + parentFolder + subFolder.getDisplayName() + "/");
        //Se ha sottocartelle lo faccio anche per loro
        if(subFolder.hasSubfolders()) {
            Vector<PSTFolder> childFolders;
            childFolders = subFolder.getSubFolders();
            for(PSTFolder childFolder : childFolders) {
                showAndGetFolders(childFolder, parentFolder + "-----", folders);
            }
        }
        
    }
    
    //Metodo che prende in input le cartelle che si vogliono analizzare
    private static String getFoldersNumber() {
        boolean ok;
        String folders;
        do
        {
            ok = true;
            System.out.println("Scrivi il numeri delle cartelle che vuoi controllare separati da virgole (,)");
            folders = Input.stringReturn();
            //Controllo che l'input sia corretto
            int i;
            for(i=0; i<folders.length(); i++) {
                if(folders.charAt(i) != ',' && !Character.isDigit(folders.charAt(i))) {
                    System.out.println("Input non valido, ripeto la domanda");
                    ok = false;
                    //Faccio il break del for, non del do-while
                    break;
                }
            } 
        }
        while(!ok);
        return folders; 
    }
    
    //Metodo che prende gli indirizzi email dalle cartelle date(controlla i numeri) e li mette nell'ArrayList<String>
    private static void getEmailAddressFromChosenFolders(ArrayList<PSTFolder> folders, String[] chosenFolders, ArrayList<String> emailAddresses) {
        //Per ogni cartella scelta estrapolo gli indirizzi email
        int i;
        for(i=0; i<chosenFolders.length; i++) {
            //Uso il try a causa del parseInt (precauzione)
            try
            {
             //Prendo l'indice della cartella
             int index = Integer.parseInt(chosenFolders[i]);
             PSTFolder folder = folders.get(index);
             getEmailAddressFromSubFolders(folder, folder.getDisplayName() + "/", emailAddresses);
            }
            catch(Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    //Metodo che prende gli indirizzi email dal file pst
    private static void getEmailAddressFromPstFile(PSTFile pstFile, ArrayList<String> emailAddresses) throws PSTException, IOException {
        System.out.println("Estrapolazione indirizzi email in corso:");
        rootFolder = pstFile.getRootFolder();
        getEmailAddressFromRootFolder(rootFolder, emailAddresses);
    }
    
    //Metodo che prende gli indirizzi email dai messaggi della rootFolder se ci sono e poi li prende anche dalle sottocartelle 
    private static void getEmailAddressFromRootFolder(PSTFolder rootFolder, ArrayList<String> emailAddresses) throws PSTException, IOException {
        //Scrivo la cartella corrente
        System.out.println("RootFolder/");
        //Se ha sottocartelle, prendo gli indirizzi email da tutte le sottocartelle
        if(rootFolder.hasSubfolders()) {
            Vector<PSTFolder> childFolders = rootFolder.getSubFolders();
            for(PSTFolder childFolder : childFolders) {
                getEmailAddressFromSubFolders(childFolder, "RootFolder/", emailAddresses);
            }
        }
        //Se ha messaggi, prendo gli indirizzi email da tutti i messaggi
        if(rootFolder.getContentCount() > 0) {
            PSTObject oggetto;
            PSTMessage message;
            do
            {
             oggetto = rootFolder.getNextChild();
             if(oggetto != null && oggetto instanceof PSTMessage) {
                message = (PSTMessage)oggetto;
                getEmailAddressFromMessage(message, emailAddresses);
             }
            }
            while(oggetto != null);
        }
    }
    
    //Metodo che prende gli indirizzi email dalle sottocartelle
    private static void getEmailAddressFromSubFolders(PSTFolder subFolder, String parentFolders, ArrayList<String> emailAddresses) throws PSTException, IOException {
        //Scrivo la cartella corrente
        System.out.println(parentFolders + subFolder.getDisplayName() + "/");
        //Se ha sottocartelle, prendo gli indirizzi email da tutte le sottocartelle
        if(subFolder.hasSubfolders()) {
            Vector<PSTFolder> childFolders = subFolder.getSubFolders();
            for(PSTFolder childFolder : childFolders) {
                getEmailAddressFromSubFolders(childFolder, parentFolders + subFolder.getDisplayName() + "/", emailAddresses);
            }
        }
        //Se ha messaggi, prendo gli indirizzi email da tutti i messaggi
        if(subFolder.getContentCount() > 0) {
            PSTObject oggetto;
            PSTMessage message;
            do
            {
             oggetto = subFolder.getNextChild();
             //Se l'oggetto non è null ed è effettivamente un messaggio
             if(oggetto != null && oggetto instanceof PSTMessage) {
                message = (PSTMessage)oggetto;
                getEmailAddressFromMessage(message, emailAddresses);
             }
            }
            while(oggetto != null);
        }
    }
        
    //Metodo che prende gli indirizzi email dal messaggio dato come parametro
    private static void getEmailAddressFromMessage(PSTMessage message, ArrayList<String> emailAddresses) throws IOException, PSTException {
        //Assegno a delle stringhe l'oggetto e il testo del messaggio
        String oggetto = message.getSubject();
        String testo = message.getBody();
        //Assegno a delle stringhe il display cc e il display to
        String displayTo = message.getDisplayCC();
        String displayCC = message.getDisplayTo();
        //Assegno a delle stringhe il primary send account
        String primarySendAccount = message.getPrimarySendAccount();
        //Assegno ad una stringa il sent representing email address
        String sentRepresentingEmailAddress = message.getSentRepresentingEmailAddress();
        //Aggiungo trovo gli indirizzi email nelle stringhe create e li assegno all'ArrayList di stringhe
        getEmailAddressFromText(oggetto, emailAddresses);
        getEmailAddressFromText(testo, emailAddresses);
        getEmailAddressFromText(displayTo, emailAddresses);
        getEmailAddressFromText(displayCC, emailAddresses);
        getEmailAddressFromText(primarySendAccount, emailAddresses);
        getEmailAddressFromText(sentRepresentingEmailAddress, emailAddresses);
        //Prendo gli indirizzi email dai destinatari
        int recipientsNumber = message.getNumberOfRecipients();
        if(recipientsNumber > 0) {
            int i;
            PSTRecipient[] recipients = new PSTRecipient[recipientsNumber];
            for(i=0; i<recipientsNumber; i++)
            {
                recipients[i] = message.getRecipient(i);
                getEmailAddressFromRecipient(recipients[i], emailAddresses);
            }
        }
        //Prendo gli indirizzi email dagli embedded messages negli allegati
        int attachmentsNumber = message.getNumberOfAttachments();
        if(recipientsNumber > 0) {
            int i;
            PSTAttachment[] attachments = new PSTAttachment[attachmentsNumber];
            for(i=0; i<attachmentsNumber; i++) {
                attachments[i] = message.getAttachment(i);
                getEmailAddressFromAttachment(attachments[i], emailAddresses);
            }
        }
        //Se il PSTMessage è un contatto, lo assegna a contact e ne estrae l'indirizzo email
        if(message instanceof PSTContact) {
            PSTContact contact;
            contact = (PSTContact) message;
            getEmailAddressFromContact(contact, emailAddresses);
        }
    }
    
    //Metodo che prende gli indirizzi email dal PSTRecipient dato come primo parametro e li passa all'ArrayList<String> dato come secondo parametro
    private static void getEmailAddressFromRecipient(PSTRecipient recipient, ArrayList<String> emailAddresses) {
        //Assegno l'indirizzo email del destinatario ad una stringa
        String emailAddress = recipient.getEmailAddress();
        //Lo metto nel ArrayList
        getEmailAddressFromText(emailAddress, emailAddresses);
    }
    
    //Metodo che prende gli indirizzi email dagli eventuali embedded messages che si trovano nell PSTAttachment dato come primo parametro e li passa all'ArrayList<String> dato come secondo parametro
    private static void getEmailAddressFromAttachment(PSTAttachment attachment, ArrayList<String> emailAddresses) throws IOException, PSTException {
        PSTMessage message;
        //Faccio un try catch per correggere un bug anomalo
        try 
        {
            message = attachment.getEmbeddedPSTMessage();
            if(message != null) {
                getEmailAddressFromMessage(message, emailAddresses);
            }
        }
        catch(Exception e) 
        {
            e.printStackTrace();
        }
    }
    
    //Metodo che prende gli indirizzi email dal contatto dato come primo parametro e li passa all'ArrayList<String> dato come secondo parametro
    private static void getEmailAddressFromContact(PSTContact contact, ArrayList<String> emailAddresses) {
        //Prendo gli indirizzi email del contatto
        String emailAddress1 = contact.getEmail1EmailAddress();
        String emailAddress2 = contact.getEmail2EmailAddress();
        String emailAddress3 = contact.getEmail3EmailAddress();
        //Prendo gli indirizzi email dalle stringhe ricevute
        getEmailAddressFromText(emailAddress1, emailAddresses);
        getEmailAddressFromText(emailAddress2, emailAddresses);
        getEmailAddressFromText(emailAddress3, emailAddresses);
    }
    
    /* METODI RIGUARDANTI IL FILE CSV */
    
    //Metodo per chiedere il nome del programma
    private static String getCsvFileName() {
        //Chiedo se vuole tenere lo stesso indirizzo del pst
        String temp;
        do
        {
         System.out.println("Vuoi creare il file csv nella stessa cartella del pst (si/no)?");
         temp = Input.stringReturn();
         if(!(temp.equals("no") || temp.equals("si"))) {
            System.out.println("Devi scrivere o \"si\" o \"no\", ripeto la domanda");
         }
        }
        while(!(temp.equals("no") || temp.equals("si")));
        String indirizzoCartella;
        //Se risponde si passo lo stesso indirizzo del pst
        if(temp.equals("si")) {
            indirizzoCartella = pathUtil;
        }
        else {
            //Chiedo di scrivere l'indirizzo della cartella che conterrà il file csv
            System.out.println("Scrivi l'indirizzo della cartella che conterrà il file csv");
            //Assegno alla stringa l'input ed aggiungo "\"
            indirizzoCartella = Input.stringReturn() + "\\";
        }
        //Chiedo di scrivere il nome del file csv
        System.out.println("Scrivi il nome del file csv di output (senza estensione)");
        //Ricevo in input il nome del file csv, aggiungo l'indirizzo e l'estensione ".csv" e lo ritorno
        return indirizzoCartella + Input.stringReturn() + ".csv";
    }
    
    //Metodo che ritorna il file csv aperto e come parametro chiede l'indirizzo del file
    private static CSVWriter openCsvFile(String fileName) throws IOException {
        //Nel caso in cui non ci siano errori ritorno il PSTFile associato al nome dato
        return new CSVWriter(new FileWriter(fileName));
    }
    
    //Metodo che scrive sul file csv le stringhe contenute nell' ArrayList indirizzi
    private static void putEmailAddressesInCsvFile(CSVWriter csvFile, ArrayList<String> emailAddresses) {
        //Definisco la variabile che conterrà il numero di indirizzi scritti
        int n = 0;
        //Se ci sono indirizzi li stampo sul file csv
        if(emailAddresses != null) {
            emailAddresses.sort(null);
            for(String emailAddress : emailAddresses)
            {
                csvFile.writeNext(new String[]{emailAddress}); //l'argomento deve essere un array di stringhe
                n++;
            }
        }
        System.out.println("Scritti nel csv " + n + " indirizzi");
    }
    
    /* METODI RIGUARDANTI LA LISTA ED IL RICONOSCIMENTO DEGLI INDIRIZZI EMAIL */
    
    //Metodo che inserisce la stringa alla lista solo se non è già presente e se non sono della blacklist
    private static void insertAddress(String address, ArrayList<String> emailAddresses) {
        //Se la lista non contiene già l'indirizzo e non è della blacklist aggiungo l'indirizzo
        if(!emailAddresses.contains(address)) {
            if(!isBlacklist(address, blacklist)) {
                emailAddresses.add(address);
            }
        }
    }
    
    //Metodo che inserisce un elemento solo se non esiste già
    private static void insertElement(String element, ArrayList<String> list) {
        //Se la lista non contiene già l'elemento aggiungo l'elemento
        if(!list.contains(element)) {
            list.add(element);
        }
    }
    
    //Metodo che controlla che l'indirizzo dato non sia della blacklist
    private static boolean isBlacklist(String emailAddress, ArrayList<String> blacklist) {
        for(String s : blacklist) {
            //Se la parte dopo la @ dell'emailAddress contiene s
            if(emailAddress.split("@")[1].contains(s)) {
                return true;
            }
        }
        return false;
    }
    
    //Metodo che controlla che il carattere sia possibile in un indirizzo email
    private static boolean isEmailAddressCharacter(char ch) {
         return Character.isLetterOrDigit((int)ch) || ch=='-' || ch=='_' || ch=='.'; //Casting a int per supportare tutti gli Unicode 
    }
    
    //Metodo che controlla che la parola sia un indirizzo email
    private static boolean isEmailAddress(String word) {
     boolean chiocciola = false;
     boolean punto = false;
     int i;
     //Se ci sono degli spazi o la stringa è vuota ritorna false
     if(word.contains(" ") || word.isEmpty()) {
         return false;
     }
     //Controllo sulla sintassi
     for(i=0; i<word.length(); i++)
     {
         if(!chiocciola) {
             if(i!=0 && word.charAt(i) == '@') {
                 chiocciola = true;
             }
             else if(!isEmailAddressCharacter(word.charAt(i))) {
                 return false;
             }
         }
         else if(!punto) {
             if(word.charAt(i - 1)!='@' && word.charAt(i)=='.') {
                 punto = true;
             }
             else if(!isEmailAddressCharacter(word.charAt(i))) {
                 return false;
             }
         }
         else {
            if(!isEmailAddressCharacter(word.charAt(i))) {
                 return false;
            } 
         }
     }
     //Se sono presenti la chiocciola ed il punto ritorno true, false altrimenti
     return chiocciola && punto;
    }
    
    //Metodo che prende gli indirizzi email da una stringa di testo e li mette nella lista del secondo parametro
    private static void getEmailAddressFromText(String s, ArrayList<String> emailAddresses) {
        //Dichiaro il contatore e la stringa temporanea
        int i, j=0;
        String temp = "";
        //Sostituisco virgole, due punti, punti e virgola, \n, \t, \, /, punti esclamativi, punti di domanda, doppi apici, apici singoli, parentesi, maggiore o minore, uguale e più con spazi
        s = s.replace(",", " ");
        s = s.replace(":", " ");
        s = s.replace(";", " ");
        s = s.replace("\n", " ");
        s = s.replace("\t", " ");
        s = s.replace("\\", " ");
        s = s.replace("/", " ");
        s = s.replace("!", " ");
        s = s.replace("?", " ");
        s = s.replace("\"", " ");
        s = s.replace("\'", " ");
        s = s.replace("(", " ");
        s = s.replace(")", " ");
        s = s.replace("[", " ");
        s = s.replace("]", " ");
        s = s.replace("{", " ");
        s = s.replace("}", " ");
        s = s.replace(">", " ");
        s = s.replace("<", " ");
        s = s.replace("=", " ");
        s = s.replace("+", " ");
        //Eseguo il controllo e l'assegnamento
        for(i=0; i<s.length(); i++) 
        {
            if(s.charAt(i) != ' ') {
                temp += s.charAt(i);
                //Se il carattere dopo è uno spazio o è l'ultimo o è "speciale"
                if(i == (s.length() - 1) || s.charAt(i + 1) == ' ' || Character.getNumericValue(s.charAt(i + 1)) == -1) {
                    if(isEmailAddress(temp)) {
                        //Se comincia per un punto lo toglie
                        if(s.startsWith(".")) {
                            s = deleteFirst(s);
                        }
                        //Se finisce per un punto lo toglie
                        if(s.endsWith(".")) {
                            s = deleteLast(s);
                        }
                        insertAddress(temp, emailAddresses);
                    }
                }
            }
            else {
                temp = "";
            }
        }
    }
    
    //Elimina il primo elemento della stringa
    private static String deleteFirst(String s) {
        return s.substring(1);
    }
    
    //Elimina il secondo elemento della stringa
    private static String deleteLast(String s) {
        return s.substring(0, s.length() - 1);
    }
}
