import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.InputMismatchException;

public class Main {
    public static void main(String[] args)
    {
        CommandLineInterface cli = new CommandLineInterface();
        ContactManager manager = new ContactManager(cli);
        int choice;
        while(true)
        {
            try
            {
                choice = cli.getMainMenu();
                if(choice < 1 || choice > 9)
                    throw new MenuChoiceException(choice);
                switch(choice)
                {
                    case 1:
                        manager.setStorageSize();
                        break;
                    case 2:
                        if(manager.getContactStorage()==null)throw new NotSizeException();
                        manager.saveToFile();
                        break;
                    case 3:
                        if(manager.getContactStorage()==null)throw new NotSizeException();
                        manager.loadFromFile();
                        break;
                    case 4:
                        if(manager.getContactStorage()==null)throw new NotSizeException();
                        choice = cli.getCreatContactMenu(manager);
                        if(choice < 1 || choice > 3)
                            throw new MenuChoiceException(choice);
                        manager.CreateContact(choice);
                        break;
                    case 5:
                        if(manager.getContactStorage()==null)throw new NotSizeException();
                        choice = cli.getSearchContactMenu(manager);
                        if(choice < 1 || choice > 5)
                            throw new MenuChoiceException(choice);
                        manager.searchContact(choice);
                        break;
                    case 6:
                        if(manager.getContactStorage()==null)throw new NotSizeException();
                        choice = cli.getDeleteContactMenu(manager);
                        if(choice < 1 || choice > 5)
                            throw new MenuChoiceException(choice);
                        manager.deleteContact(choice);
                        break;
                    case 7:
                        if(manager.getContactStorage()==null)throw new NotSizeException();
                        choice = cli.getEditContactMenu();
                        if(choice < 1 || choice > 5)
                            throw new MenuChoiceException(choice);
                        manager.editContact(choice);
                        break;
                    case 8:
                        if(manager.getContactStorage()==null)throw new NotSizeException();
                        cli.printContactInfo(manager.getContactStorage().getContacts());
                        break;
                    case 9:
                        break;
                }
                if (choice == 9)
                {break;}
            }
            catch(MenuChoiceException e)
            {cli.printErrorMessage(e);}
            catch(SizeException e)
            {cli.printErrorMessage(e);}
            catch(FormatException e)
            {cli.printErrorMessage(e);}
            catch(FileNotFoundException e)
            {cli.printErrorMessage(e);}
            catch(IOException e)
            {cli.printErrorMessage(e);}
            catch(NotSizeException e)
            {cli.printErrorMessage(e);}
            catch(InputMismatchException e)
            {
                cli.printErrorMessage(e);
                System.exit(0);
            }
        }
    }
}
