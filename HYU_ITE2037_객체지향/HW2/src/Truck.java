import java.util.ArrayList;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class Truck {
    private int ID;
    private double fuel;
    private Warehouse currentWarehouse;
    private int realWeight;
    private int totalWeight;
    private int maxNumOfAllCargoes;
    private int maxNumofHeavyCargoes;
    private int maxNumofDangerousCargoes;
    private int maxNumofLiquidCargoes;
    private double fuelPerKm;
    private ArrayList<Cargo> currentCargoes;

    public Truck(){}
    public Truck(int ID, Warehouse w, int totalWeight, int maxNumOfAllCargoes,
                 int maxNumofHeavyCargoes, int maxNumofDangerousCargoes,
                 int maxNumofLiquidCargoes, double fuelPerKm) {
        this.ID = ID;
        this.currentWarehouse = w;
        this.totalWeight = totalWeight;
        this.maxNumOfAllCargoes = maxNumOfAllCargoes;
        this.maxNumofHeavyCargoes = maxNumofHeavyCargoes;
        this.maxNumofDangerousCargoes = maxNumofDangerousCargoes;
        this.maxNumofLiquidCargoes = maxNumofLiquidCargoes;
        this.fuelPerKm = fuelPerKm;
        realWeight=0;
        currentCargoes = new ArrayList<>();
    }

    public double getFuel(){return fuel;}
    public void reFuel(double amount){
        fuel = fuel + amount;
    }
    public int getID(){return ID;}
    public Warehouse getCurrentWarehouse(){return currentWarehouse;}

    public ArrayList<Cargo>getCurrentCargoes(){
        for(int i=0;i<currentCargoes.size()-1;i++){
            for(int j=0; j<currentCargoes.size()-1-i; j++){
                if(currentCargoes.get(j).getID() > currentCargoes.get(j+1).getID()){
                    Cargo cmp = currentCargoes.get(j);
                    currentCargoes.set(j, currentCargoes.get(j+1));
                    currentCargoes.set(j+1, cmp);
                }
            }
        }
        return currentCargoes;
    }

    public boolean sailTo(Warehouse w){
        double dis = currentWarehouse.getDistance(w);
        double fuelConsumption = 0;
        double cargoFuelConsumption = 0;

        for(int i=0; i<currentCargoes.size();i++) {
            cargoFuelConsumption += currentCargoes.get(i).consumption();
        }
        fuelConsumption = dis * fuelPerKm + cargoFuelConsumption *dis;
        if(fuel>=fuelConsumption){
            fuel -= fuelConsumption;
            currentWarehouse.outgoingTruck(this);
            currentWarehouse = w;
            w.incomingTruck(this);
            return true;
        }
        return false;
    }

    public boolean load(Cargo c){
        if(currentCargoes.size() == maxNumOfAllCargoes){
            return false;
        }
        if(realWeight + c.getWeight() > totalWeight){
            return false;
        }

        int heavy = 0,danger = 0,liquid = 0;
        for(Cargo cargo : currentCargoes)
        {
            if(cargo instanceof HeavyCargo) {
                if(cargo instanceof DangerousCargo) {danger++;heavy++;}
                else if(cargo instanceof LiquidCargo) {liquid++;heavy++;}
                else heavy++;
            }
        }
        if (c instanceof HeavyCargo){
            if(heavy == maxNumofHeavyCargoes) {
                return false;
            }else if(c instanceof DangerousCargo) {
                if (danger == maxNumofDangerousCargoes){
                    return false;
                }
            }else if(c instanceof LiquidCargo) {
                if (liquid == maxNumofLiquidCargoes){
                    return false;
                }
            }
        }

        for(int i=0;i<currentWarehouse.getCargoes().size();i++){    // seraching Cargo will be loaded in warehouse
            if (currentWarehouse.getCargoes().get(i).getID() == c.getID()){
                currentCargoes.add(currentWarehouse.getCargoes().get(i));
                currentWarehouse.getCargoes().remove(i);
                realWeight += c.getWeight();
                return true;
            }
        }
        return false;
    }

    public boolean unload(Cargo c){
        for(int i=0;i<currentCargoes.size();i++) {  // search cargo will be unloaded in Truck
            if (currentCargoes.get(i).getID() == c.getID()) {
                currentWarehouse.getCargoes().add(currentCargoes.get(i));
                currentCargoes.remove(i);
                return true;
            }
        }
        return false;
    }

    public boolean equals(Truck other){
        if(getID() == other.getID()) return true;
        return false;
    }
    public void printTruck(ArrayList<ArrayList<Cargo>> cargo, String fileName) {
        try {
            PrintStream out = new PrintStream(new FileOutputStream(fileName,true));
            out.printf("##Truck%d: %.1f%n", getID(), getFuel());
            if (cargo.get(0).size() != 0) {
                out.print("####BasicCargo: ");
                for (Cargo c : cargo.get(0)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            if (cargo.get(1).size() != 0) {
                out.print("####HeavyCargo: ");
                for (Cargo c : cargo.get(1)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            if (cargo.get(2).size() != 0) {
                out.print("####DangerousCargo: ");
                for (Cargo c : cargo.get(2)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            if (cargo.get(3).size() != 0) {
                out.print("####LiquidCargo: ");
                for (Cargo c : cargo.get(3)) {
                    out.print(c.getID() + " ");
                }
                out.println();
            }
            out.close();
        } catch(FileNotFoundException e){ e.printStackTrace();}
    }
}
