import java.util.Scanner;
import java.util.StringTokenizer;
import java.util.ArrayList;
import java.io.File;
import java.io.PrintStream;
import java.io.FileNotFoundException;

public class Main {
    public static void main(String[] args) {

        try {
            int eventCap = 0, event = 0, cID = 0, wID = 0, tID = 0, cWeight = 0;
            String special = null;
            String fileName = args[1];
            ArrayList<Warehouse> warehouses = new ArrayList<>();
            ArrayList<Cargo> cargoes = new ArrayList<>();
            ArrayList<Truck> trucks = new ArrayList<>();
            Scanner in = new Scanner(new File(args[0]));
            PrintStream out = new PrintStream(new File(args[1]));
            String input;
            StringTokenizer stk;

            eventCap = in.nextInt();
            in.nextLine();

            for (int i = 0; i < eventCap; i++) {
                if(!in.hasNextLine()) break;
                input = in.nextLine();
                stk = new StringTokenizer(input);
                event = Integer.parseInt(stk.nextToken());

                if (event == 1) // create Cargo into Warehouse
                {
                    Cargo c = null;
                    wID = Integer.parseInt(stk.nextToken());
                    for (Warehouse w : warehouses) {
                        if (w.getID() == wID) {
                            cWeight = Integer.parseInt(stk.nextToken());
                            if (stk.hasMoreTokens()) {
                                special = stk.nextToken();
                                if (special.equals("D")) {
                                    c = new DangerousCargo(cargoes.size(), cWeight);
                                } else if (special.equals("L")) {
                                    c = new LiquidCargo(cargoes.size(), cWeight);
                                } else {
                                    break;
                                }
                            } else {
                                if (cWeight > 0 && cWeight <= 1000) {
                                    c = new BasicCargo(cargoes.size(), cWeight);
                                } else if (cWeight > 1000) {
                                    c = new HeavyCargo(cargoes.size(), cWeight);
                                } else {
                                    break;
                                }
                            }
                            cargoes.add(c);
                            w.getCargoes().add(c);
                            break;
                        }
                    }
                } else if (event == 2)    // create Truck into warehouse
                {
                    int totalWeight;
                    int maxNumOfAllCargoes;
                    int maxNumofHeavyCargoes;
                    int maxNumofDangerousCargoes;
                    int maxNumofLiquidCargoes;
                    double fuelPerKm;
                    Truck truck = null;
                    wID = Integer.parseInt(stk.nextToken());
                    totalWeight = Integer.parseInt(stk.nextToken());
                    maxNumOfAllCargoes = Integer.parseInt(stk.nextToken());
                    maxNumofHeavyCargoes = Integer.parseInt(stk.nextToken());
                    maxNumofDangerousCargoes = Integer.parseInt(stk.nextToken());
                    maxNumofLiquidCargoes = Integer.parseInt(stk.nextToken());
                    fuelPerKm = Double.parseDouble(stk.nextToken());

                    for (Warehouse w : warehouses) {
                        if (w.getID() == wID) {
                            truck = new Truck(trucks.size(), w, totalWeight, maxNumOfAllCargoes,
                                    maxNumofHeavyCargoes, maxNumofDangerousCargoes,
                                    maxNumofLiquidCargoes, fuelPerKm);
                            w.incomingTruck(truck);
                            trucks.add(truck);
                            break;
                        }
                    }
                } else if (event == 3)    // crete Warehouse
                {
                    double x = 0, y = 0;
                    x = Double.parseDouble(stk.nextToken());
                    y = Double.parseDouble(stk.nextToken());
                    Warehouse warehouse = new Warehouse(i, x, y);
                    warehouses.add(warehouse);
                } else if (event == 4)    // load Cargo from warehouse into Truck
                {
                    tID = Integer.parseInt(stk.nextToken());
                    cID = Integer.parseInt(stk.nextToken());
                    for (Truck truck : trucks) {
                        if (truck.getID() == tID) {
                            for (Cargo cargo : cargoes) {
                                if (cargo.getID() == cID) {
                                    truck.load(cargo);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                } else if (event == 5)    // unload Cargo from Truck into warehouse
                {
                    tID = Integer.parseInt(stk.nextToken());
                    cID = Integer.parseInt(stk.nextToken());
                    for (Truck truck : trucks) {
                        if (truck.getID() == tID) {
                            for (Cargo cargo : cargoes) {
                                if (cargo.getID() == cID) {
                                    truck.unload(cargo);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                } else if (event == 6)    // Truck moves to another warehouse
                {
                    tID = Integer.parseInt(stk.nextToken());
                    wID = Integer.parseInt(stk.nextToken());
                    for (Truck truck : trucks) {
                        if (truck.getID() == tID) {
                            for (Warehouse ware : warehouses) {
                                if (ware.getID() == wID && truck.getCurrentWarehouse().getID() != wID) {
                                    truck.sailTo(ware);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                } else if (event == 7)    // refuel a truck
                {
                    double fuel = 0;
                    tID = Integer.parseInt(stk.nextToken());
                    fuel = Double.parseDouble(stk.nextToken());
                    for (Truck truck : trucks) {
                        if (truck.getID() == tID) {
                            truck.reFuel(fuel);
                            break;
                        }
                    }
                }
            }
            for (Warehouse w : warehouses) {
                ArrayList<ArrayList<Cargo>> cargo = Cargo.sortedCargoes(w.getCargoes());
                w.printWarehouse(cargo,fileName);
            }
            in.close();
            out.close();
        } catch(FileNotFoundException e){
            e.printStackTrace();
        }
    }
}