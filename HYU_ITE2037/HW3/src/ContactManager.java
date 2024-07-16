import java.util.ArrayList;
import java.io.FileNotFoundException;
import java.util.StringTokenizer;
import java.io.IOException;
import java.util.InputMismatchException;

public class ContactManager {
    private MyStorage<ContactInfo> contactStorage;
    private CommandLineInterface cli;

    public ContactManager(){}
    public ContactManager(CommandLineInterface inface)
    {cli = inface;}

    public MyStorage<ContactInfo> getContactStorage(){
        return contactStorage;
    }

    public void setStorageSize()throws InputMismatchException{
        if(contactStorage == null) {
            contactStorage = new MyStorage<>(cli.getSetSizeMenu());
            return;
        }
        int size = cli.getSetSizeMenu();

        if(size<=contactStorage.getSize()) {
            System.out.println("Resize fail!!");
            return;
        }
        contactStorage.setSize(size);
    }

    public void saveToFile()throws FileNotFoundException
    {
        cli.saveContactToFile(contactStorage.getContacts());
    }

    public void loadFromFile()throws IOException
    {
        cli.getContactsFromFile(contactStorage);
    }

    public void CreateContact(int choice)throws FormatException{

        StringTokenizer stk = new StringTokenizer(cli.getContactInfo(choice));
        String name = stk.nextToken();
        String number = stk.nextToken();
        String any = stk.nextToken();

        for(int i=0;i<contactStorage.getContacts().size();i++){
            if(number.equals(contactStorage.getContacts().get(i).getNumber()))
                return;
        }

        if(choice == 1){
            NormalContact n = new NormalContact(name,number,any);
            contactStorage.getContacts().add(n);
        }
        else if(choice == 2){
            ClubContact c = new ClubContact(name,number,any);
            contactStorage.getContacts().add(c);
        }
        else if(choice == 3){
            DepartmentContact d = new DepartmentContact(name,number,any);
            contactStorage.getContacts().add(d);
        }
    }

    public ContactInfo searchContact(int choice){
        String info = cli.getSearchInfo(choice);
        ContactInfo cnt = null;
        switch(choice){
            case 1:
                for(int i=0;i<contactStorage.getContacts().size();i++) {
                    if(info.equals(contactStorage.getContacts().get(i).getName())){
                        cnt = contactStorage.getContacts().get(i);
                        System.out.println(contactStorage.getContacts().get(i).getInfo());
                        return cnt;
                    }
                }
                break;
            case 2:
                for(int i=0;i<contactStorage.getContacts().size();i++) {
                    if(info.equals(contactStorage.getContacts().get(i).getNumber())){
                        cnt = contactStorage.getContacts().get(i);
                        System.out.println(contactStorage.getContacts().get(i).getInfo());
                        return cnt;
                    }
                }
                break;
            case 3:
                for(int i=0;i<contactStorage.getContacts().size();i++) {
                    if(contactStorage.getContacts().get(i) instanceof NormalContact) {
                        NormalContact n = (NormalContact)contactStorage.getContacts().get(i);
                        if (info.equals(n.getRelation())) {
                            cnt = contactStorage.getContacts().get(i);
                            System.out.println(contactStorage.getContacts().get(i).getInfo());
                            return cnt;
                        }
                    }
                }
                break;
            case 4:
                for(int i=0;i<contactStorage.getContacts().size();i++) {
                    if(contactStorage.getContacts().get(i) instanceof ClubContact) {
                        ClubContact c = (ClubContact)contactStorage.getContacts().get(i);
                        if (info.equals(c.getClubName())) {
                            cnt = contactStorage.getContacts().get(i);
                            System.out.println(contactStorage.getContacts().get(i).getInfo());
                            return cnt;
                        }
                    }
                }
                break;
            case 5:
                for(int i=0;i<contactStorage.getContacts().size();i++) {
                    if(contactStorage.getContacts().get(i) instanceof DepartmentContact) {
                        DepartmentContact d = (DepartmentContact)contactStorage.getContacts().get(i);
                        if (info.equals(d.getDepartment())) {
                            cnt = contactStorage.getContacts().get(i);
                            System.out.println(contactStorage.getContacts().get(i).getInfo());
                            return cnt;
                        }
                    }
                }
                break;
        }
        if(cnt == null)
            System.out.println("Don't exist contact!!");
        return cnt;
    }

    public void deleteContact(int choice){
        ContactInfo info = searchContact(choice);
        if(info == null) return;        // 못찾은 경우

        contactStorage.getContacts().remove(info);
        cli.successDelete();
    }

    public void editContact(int choice)throws InputMismatchException, FormatException
    {
        ContactInfo info = searchContact(choice);
        if(info == null) return;
        int menu;
        String edit = null;
        if(info instanceof NormalContact){
            menu = cli.editContact(1);
            edit = cli.edit(menu);
            if(menu == 1)
                info.setName(edit);
            else if(menu == 2)
                info.setNumber(edit);
            else{
                NormalContact n = (NormalContact)info;
                n.setRelation(edit);
            }
        }
        else if (info instanceof ClubContact)
        {
            menu = cli.editContact(2);
            edit = cli.edit(menu);
            if(menu == 1)
                info.setName(edit);
            else if(menu == 2)
                info.setNumber(edit);
            else{
                ClubContact c = (ClubContact)info;
                c.setClubName(edit);
            }
        }
        else{
            menu = cli.editContact(3);
            edit = cli.edit(menu);
            if(menu == 1)
                info.setName(edit);
            else if(menu == 2)
                info.setNumber(edit);
            else{
                DepartmentContact d = (DepartmentContact)info;
                d.setDepartment(edit);
            }
        }
    }

    public class MyStorage<T>{
        private int size;
        private ArrayList<T> contacts;

        public MyStorage(){}
        public MyStorage(int size){
            this.size = size;
            contacts = new ArrayList<>();
        }

        public int getSize(){return size;}
        public void setSize(int size){this.size = size;}
        public ArrayList<T> getContacts(){return contacts;}
    }
}
