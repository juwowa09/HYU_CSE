import java.util.ArrayList;

public abstract class Cargo {
    protected int ID;
    protected int weight;

    public Cargo(){}
    public Cargo(int ID, int weight){
        this.ID = ID;
        this.weight = weight;
    }
    // return fuel comsumption required by the cargo
    public abstract double consumption();
    public double getWeight()
    { return weight; }
    public int getID()
    { return ID; }

    public static ArrayList<ArrayList<Cargo>> sortedCargoes(ArrayList<Cargo> cargoes)
    {
        ArrayList<ArrayList<Cargo>> c = new ArrayList<>();
        ArrayList<Cargo> B = new ArrayList<>();
        ArrayList<Cargo> H = new ArrayList<>();
        ArrayList<Cargo> D = new ArrayList<>();
        ArrayList<Cargo> L = new ArrayList<>();

        for(int i=0;i<cargoes.size()-1;i++){
            for(int j=0; j<cargoes.size()-1-i; j++){
                if(cargoes.get(j).getID() > cargoes.get(j+1).getID()){
                    Cargo cmp = cargoes.get(j);
                    cargoes.set(j, cargoes.get(j+1));
                    cargoes.set(j+1, cmp);
                }
            }
        }

        for(int i=0;i<cargoes.size();i++) {
            if(cargoes.get(i) instanceof BasicCargo){
                B.add(cargoes.get(i));
            }
            else if(cargoes.get(i) instanceof DangerousCargo){
                D.add(cargoes.get(i));
            }
            else if (cargoes.get(i) instanceof LiquidCargo) {
                L.add(cargoes.get(i));
            }
            else if(cargoes.get(i) instanceof HeavyCargo) {
                H.add(cargoes.get(i));
            }
        }
        c.add(B);c.add(H);c.add(D);c.add(L);
        return c;
    }
}
