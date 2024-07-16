import java.util.ArrayList;
import java.io.PrintStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;

public class Warehouse {
    private int ID;
    private double X, Y;
    private ArrayList<Cargo> cargoes;
    private ArrayList<Truck> history;
    private ArrayList<Truck> current;
    private ArrayList<ArrayList<Cargo>> sortedCargoes;

    public Warehouse() {
    }

    public Warehouse(int ID, double X, double Y) {
        this.ID = ID;
        this.X = X;
        this.Y = Y;
        cargoes = new ArrayList<>();
        history = new ArrayList<>();
        current = new ArrayList<>();
    }

    public int getID() {
        return ID;
    }

    public double getX() {
        return X;
    }

    public double getY() {
        return Y;
    }

    public ArrayList<Truck> getCurrent() {
        return current;
    }

    public double getDistance(Warehouse other) {
        double x = getX() - other.getX();
        double y = getY() - other.getY();
        double dis = x * x + y * y;
        return Math.sqrt(dis);
    }

    public void incomingTruck(Truck t) {
        for (int i = 0; i < current.size(); i++) {
            if (current.get(i).equals(t))    // check duplicate
                return;
        }
        current.add(t);
    }

    public void outgoingTruck(Truck t) {
        for (int i = 0; i < current.size(); i++) {
            if (current.get(i).equals(t)) {
                current.remove(i);
                break;
            }
        }
        for (int i = 0; i < history.size(); i++) {
            if (history.get(i).equals(t))    // check duplicate
                return;
        }
        history.add(t);
    }

    public void sortTruck() {
        for (int i = 0; i < current.size() - 1; i++) {
            for (int j = 0; j < current.size() - 1 - i; j++) {
                if (current.get(j).getID() > current.get(j + 1).getID()) {
                    Truck cmp = current.get(j);
                    current.set(j, current.get(j + 1));
                    current.set(j + 1, cmp);
                }
            }
        }
    }

    public ArrayList<Cargo> getCargoes() {
        return cargoes;
    }

    public void printWarehouse(ArrayList<ArrayList<Cargo>> cargo, String fileName) {
        try {
            PrintStream out = new PrintStream(new FileOutputStream(fileName,true));

            out.printf("Warehouse%d: (%.1f, %.1f)%n", getID(), getX(), getY());
            if (cargo.get(0).size() != 0) {
                out.print("##BasicCargo: ");
                for (Cargo c : cargo.get(0)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            if (cargo.get(1).size() != 0) {
                out.print("##HeavyCargo: ");
                for (Cargo c : cargo.get(1)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            if (cargo.get(2).size() != 0) {
                out.print("##DangerousCargo: ");
                for (Cargo c : cargo.get(2)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            if (cargo.get(3).size() != 0) {
                out.print("##LiquidCargo: ");
                for (Cargo c : cargo.get(3)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            sortTruck();
            for (Truck t : current) {
                t.printTruck(Cargo.sortedCargoes(t.getCurrentCargoes()), fileName);
            }
            out.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
