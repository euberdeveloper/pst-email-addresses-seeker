# pst-email-addresses-seeker
A Java application which, given a pst file, analyzes all the emails and return every email address found. These are taken from contacts, email subject, email text, email cc and email sender. A blacklilst of email domains or a folders targeted test could be performed too.

## Project purpose
This project was made in 2016 with Java NetBeans and not update since then. A [PST](https://en.wikipedia.org/wiki/Personal_Storage_Table) file is an archive file for Microsoft Outlook, which stores emails and contacts. The problem was this: given a PST file, make a program which fetch all email addresses, from contacts or emails' subject/text/cc/sender/attachments.

## Project usage
__source__ contains the Java NetBeans project, while __dist__ the built program. The program is in Italian. After being opened, the program asks if you want to create or update a blacklist csv which contains all the email domains to skip. After that it asks for the path of the folder and then the name without extension of the pst file to read. Then it asks if you want to scan the full pst file or only some directories. In conclusion it asks where to save the csv result and saves it.
