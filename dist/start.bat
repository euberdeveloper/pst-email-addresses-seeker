@echo off
echo Questo programma serve a prendere gli indirizzi email contenuti in un file pst. Li prende da contatti, oggetto, testo, cc, mittente.
echo All'inizio chiede di creare o aggiornare una blacklist che dice i domini da escludere(non considerare) nella creazione della lista.
echo Poi chiede di specificare il percorso del file pst da elaborare ed il nome senza estensione.
echo Infine chiede di specificare il percorso del file csv da elaborare dove saranno scritti gli indirizzi email ed il nome senza estensione
echo.
java -jar estrapola_indirizzi_email_da_file_pst.jar