import java.util.Scanner;
import java.util.ArrayList;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.PrintStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.StringTokenizer;
import java.util.InputMismatchException;

public class CommandLineInterface {
    private Scanner scanner;

    public CommandLineInterface(){
        scanner = new Scanner(System.in);
    }

    public int getMainMenu()throws InputMismatchException{
        System.out.println();
        System.out.println("----<Main Menu>----");
        System.out.println("1. Set size of total contacts");
        System.out.println("2. Save the current contact to a file");
        System.out.println("3. Load the saved contact file");
        System.out.println("4. Register new contact");
        System.out.println("5. Search contact");
        System.out.println("6. Delete contact");
        System.out.println("7. Edit contact");
        System.out.println("8. View All Contacts");
        System.out.println("9. Exit");
        System.out.print("Select: ");
        return scanner.nextInt();
    }

    public int getSetSizeMenu()throws InputMismatchException
    {
        System.out.print("Input size of total contacts: ");
        return scanner.nextInt();
    }

    public int getCreatContactMenu(ContactManager manager)throws SizeException, InputMismatchException
    {
        if(manager.getContactStorage().getSize() == manager.getContactStorage().getContacts().size())
            throw new SizeException();
        System.out.println();
        System.out.println("Create contact...");
        System.out.println("1. normal contact");
        System.out.println("2. club contact");
        System.out.println("3. department contact");
        System.out.print("Select: ");
        return scanner.nextInt();
    }

    public String getContactInfo(int choice)throws FormatException
    {
        String name;
        String number;
        String any = null;

        System.out.print("Input name: ");
        scanner.nextLine();
        name = scanner.nextLine();
        System.out.print("Input Phone Number: ");
        number = scanner.nextLine();

        if(choice == 1) {
            System.out.print("Input Relation: ");
            any = scanner.nextLine();
            if((name.length()<3 || name.length()>20) || (any.length() <3 || any.length()>20)
                    || (!number.matches("010-\\d{4}-\\d{4}")))
                throw new FormatException();
        }
        else if(choice == 2){
            System.out.print("Input Club Name: ");
            any = scanner.nextLine();
            if((name.length()<3 || name.length()>20) || (any.length() <3 || any.length()>20)
                    || (!number.matches("010-\\d{4}-\\d{4}")))
                throw new FormatException();
        }
        else if(choice == 3){
            System.out.print("Input Department: ");
            any = scanner.nextLine();
            if((name.length()<3 || name.length()>20) || (any.length() <3 || any.length()>20)
                    || (!number.matches("010-\\d{4}-\\d{4}")))
                throw new FormatException();
        }
        return name+ " " +" " +number+" "+any;
    }

    public int getSearchContactMenu(ContactManager manager)throws SizeException, InputMismatchException
    {
        if(manager.getContactStorage().getSize() == manager.getContactStorage().getContacts().size())
            throw new SizeException();
        System.out.println();
        System.out.println("Search contact...");
        System.out.println("Choose the variable");
        System.out.println("1. name");
        System.out.println("2. phone number");
        System.out.println("3. relation");
        System.out.println("4. club name");
        System.out.println("5. department");
        System.out.print("Select: ");
        return scanner.nextInt();
    }

    public String getSearchInfo(int choice)
    {
        String search = scanner.nextLine();
        switch(choice){
            case 1:
                System.out.print("Input name: ");
                search = scanner.nextLine();
                break;
            case 2:
                System.out.print("Input phone number: ");
                search = scanner.nextLine();
                break;
            case 3:
                System.out.print("Input relation: ");
                search = scanner.nextLine();
                break;
            case 4:
                System.out.print("Input club name: ");
                search = scanner.nextLine();
                break;
            case 5:
                System.out.print("Input department: ");
                search = scanner.nextLine();
                break;
        }
        return search;
    }

    public int getDeleteContactMenu(ContactManager manager) throws SizeException, InputMismatchException
    {
        if(manager.getContactStorage().getContacts().size() == 0)
            throw new SizeException();
        System.out.println();
        System.out.println("Delete contact...");
        System.out.println("Choose the variable");
        System.out.println("1. name");
        System.out.println("2. phone number");
        System.out.println("3. relation");
        System.out.println("4. club name");
        System.out.println("5. department");
        System.out.print("Select: ");
        return scanner.nextInt();
    }
    public void successDelete()
    {System.out.println("Contact Delete complete!");}

    public int getEditContactMenu()throws InputMismatchException
    {
        System.out.println();
        System.out.println("Search Edit contact...");
        System.out.println("Choose the variable");
        System.out.println("1. name");
        System.out.println("2. phone number");
        System.out.println("3. relation");
        System.out.println("4. club name");
        System.out.println("5. department");
        System.out.print("Select: ");
        return scanner.nextInt();
    }
    public int editContact(int choice)throws InputMismatchException
    {
        System.out.println();
        System.out.println("Edit contact...");
        System.out.println("Choose the variable");
        System.out.println("1. name");
        System.out.println("2. phone number");

        if(choice == 1)
        {
            System.out.println("3. relation");
        }
        else if (choice == 2){
            System.out.println("3. club name");
        }
        else{
            System.out.println("3. department");
        }
        System.out.print("Select: ");
        return scanner.nextInt();
    }
    public String edit(int choice)throws FormatException{
        scanner.nextLine();
        String edit;
        System.out.print("edit contact: ");

        if(choice == 2) {
            edit = scanner.nextLine();
            if((!edit.matches("010-\\d{4}-\\d{4}")))
            throw new FormatException();
        }
        else {
            edit = scanner.nextLine();
            if ((edit.length() < 3 || edit.length() > 20))
                throw new FormatException();
        }
        return edit;
    }


    public void printContactInfo(ArrayList<ContactInfo> arr)
    {
        int cnt = 0;
        System.out.println();
        System.out.println("Normal Contacts");
        for(int i=0;i<arr.size();i++)
        {
            if(arr.get(i) instanceof NormalContact){
                cnt++;
                System.out.println("\t" + cnt + arr.get(i).toString());
            }
        }
        System.out.println("Club Contacts");
        cnt = 0;
        for(int i=0;i<arr.size();i++)
        {
            if(arr.get(i) instanceof ClubContact){
                cnt++;
                System.out.println("\t" + cnt + arr.get(i).toString());
            }
        }
        System.out.println("Department Contacts");
        cnt = 0;
        for(int i=0;i<arr.size();i++)
        {
            if(arr.get(i) instanceof DepartmentContact){
                cnt++;
                System.out.println("\t" + cnt + arr.get(i).toString());
            }
        }
    }

    public void printErrorMessage(Exception e)
    {System.out.println(e.getMessage());}

    public void saveContactToFile(ArrayList<ContactInfo> cInfo)throws FileNotFoundException
    {
        int cnt = 0;
        PrintStream out = new PrintStream(new FileOutputStream("ContactBook.txt",true));
        for(int i=0;i<cInfo.size();i++)
        {
            out.println(cInfo.get(i).getInfo());
        }
//        out.println("Normal Contacts");
//        for(int i=0;i<cInfo.size();i++)
//        {
//            if(cInfo.get(i) instanceof NormalContact){
//                cnt++;
//                out.println("\t" + cnt + cInfo.get(i).toString());
//            }
//        }
//        cnt = 0;
//        out.println("Club Contacts");
//        for(int i=0;i<cInfo.size();i++)
//        {
//            if(cInfo.get(i) instanceof ClubContact){
//                cnt++;
//                out.println("\t" + cnt + cInfo.get(i).toString());
//            }
//        }
//        cnt = 0;
//        out.println("Normal Contacts");
//        for(int i=0;i<cInfo.size();i++)
//        {
//            if(cInfo.get(i) instanceof DepartmentContact){
//                cnt++;
//                out.println("\t" + cnt + cInfo.get(i).toString());
//            }
//        }
        out.close();
    }

    public void getContactsFromFile(ContactManager.MyStorage<ContactInfo> cInfo)throws IOException{
        BufferedReader reader = new BufferedReader(new FileReader("ContactBook.txt"));

        String info;
        String name;
        String number;
        String any;

        while((info = reader.readLine())!= null && cInfo.getSize() != cInfo.getContacts().size()){
//            if(info.equals("Normal Contacts"))
//                continue;
//
//            else if (info.equals("Club Contacts"))
//                continue;
//
//            else if (info.equals("Department Contacts"))
//                continue;

            StringTokenizer stk = new StringTokenizer(info);

            while(!stk.nextToken().equals("name:")){}
            name = stk.nextToken();

            while(!stk.nextToken().equals("number:")){}
            number = stk.nextToken();
            stk.nextToken();

            int cnt = 0;
            for(int i=0;i<cInfo.getContacts().size();i++) {
                if(number.equals(cInfo.getContacts().get(i).getNumber()))
                    cnt++;
            }
            if(cnt!=0)continue;

            any = stk.nextToken();
            if(any.equals("relation:")){
                NormalContact n = new NormalContact(name, number, stk.nextToken());
                cInfo.getContacts().add(n);
            }
            else if(any.equals("club")){
                stk.nextToken();
                ClubContact c = new ClubContact(name,number,stk.nextToken());
                cInfo.getContacts().add(c);
            }
            else if(any.equals("department:"))
            {
                DepartmentContact d = new DepartmentContact(name, number, stk.nextToken());
                cInfo.getContacts().add(d);
            }
        }
    reader.close();
    }
}
