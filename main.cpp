#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;
class UniqueID {
protected:
    static int nextID;
public:
    int id;
    UniqueID( );
    UniqueID(const UniqueID& orig);
    UniqueID& operator=(const UniqueID& orig);
    friend class CarBuilder;
    friend class Car;
};
int UniqueID::nextID=0;

UniqueID::UniqueID( ) {
    id = ++nextID;
}

UniqueID::UniqueID(const UniqueID& orig) {
    id = orig.id;
}

UniqueID& UniqueID::operator=(const UniqueID& orig) {
    id = orig.id;
    return(*this);
}

struct clonable {
    virtual ~clonable() {}
    virtual clonable* clone() const = 0;
};

class Car:public clonable
{
    UniqueID id;
    int seatsnr;
    int doorsnr;
    int trunksize;
    string colour;
    friend class CarBuilder;
    friend class ID;
public:
    Car(int snr=-1,int dnr=-1,int trunks=-1, string col="none"):seatsnr(snr),doorsnr(dnr),trunksize(trunks),colour(col)
    {
    }
    Car(const Car& c)=default;
    virtual ~Car()=default;
    ///---------------GET-SET----------------------
    int getID() const
    {
        return id.id;
    }
    void setSeats(const int nr)
    {
        seatsnr=nr;
    }
    void setDoors(const int nr)
    {
        doorsnr=nr;
    }
    void setTrunk(const int nr)
    {
        trunksize=nr;
    }
    void setColour(const string& clr)
    {
        colour=clr;
    }
    void setID()
    {
        id.id=++UniqueID::nextID;
    }
    int getSeats() const
    {
        return seatsnr;
    }
    int getDoors() const
    {
        return doorsnr;
    }
    int getTrunk() const
    {
        return trunksize;
    }
    string getColour() const
    {
        return colour;
    }
    virtual Car* clone() const
    {
        return (new Car(*this));
    }
    virtual void print(ostream& os) const
    {
        os<<"MASINA CU NUMARUL "<<id.id<<endl;
        os<<"NUMAR LOCURI: "<<seatsnr<<endl;
        os<<"NUMAR USI: "<<doorsnr<<endl;
        os<<"CAPACITATE PORTBAGAJ: "<<trunksize<<endl;
        os<<"CULOARE: "<<colour<<endl;
    }
    ///-------------------------OPERATORI--------------------------------
    bool operator >(const Car &rhs)
    {
        return trunksize>rhs.trunksize;
    }
    Car& operator=(const Car &rhs) {

        if (this != &rhs)
        {
            setSeats(rhs.seatsnr);
            setTrunk(rhs.trunksize);
            setDoors(rhs.doorsnr);
            setColour(rhs.colour);

        }


        return *this;
    }
    friend ostream& operator<<(ostream &os, const Car& n){
        n.print(os);
        os<<"----------------------------------------------------------"<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, Car& n) {
        cout << "Introduceti numar locuri: ";
        is >> n.seatsnr;
        cout << endl;
        cout << "Introduceti numar usi: ";
        is >> n.doorsnr;
        cout << endl;
        cout << "Introduceti marimea portbagajului: ";
        is >> n.trunksize;
        cout << endl;
        cout << "Introduceti culoarea:";
        is >> n.colour;
        cout << endl;
        return is;

    }

};

///BUILDER DESIGN--------------------------------------------------------------
class CarBuilder
{
    Car c;
public:
    CarBuilder() = default;
    CarBuilder& seatsnr(int nr)
    {
        c.seatsnr=nr;
        return *this;
    }
    CarBuilder& doorsnr(int nr)
    {
        c.doorsnr=nr;
        return *this;
    }
    CarBuilder& trunksize(int nr)
    {
        c.trunksize=nr;
        return *this;
    }
    CarBuilder& colour(string nr)
    {
        c.colour=nr;
        return *this;
    }
    CarBuilder& id()
    {
        c.id.id=++UniqueID::nextID;
        return *this;
    }
    Car build()
    {
        return c;
    }
};

///--------------FACTORY DESIGN--------------
class CarFactory
{
public:
    static Car BMW()
    {
        return Car(4,4,70,"black");
    }
    static Car AUDI()
    {
        return Car(3,2,35,"silver");
    }
    static Car CITROEN()
    {
        return Car(7,4,100,"white");
    }
};

///----------------Clasa mostenita---------------
class Bus:public Car
{
    int tkprice;
    int passnr;
public:
    Bus(int snr=-1, int dnr=-1, int ts=-1, string clr="none", int tp=-1, int pnr=-1):Car(snr,dnr,ts,clr),tkprice(tp),passnr(pnr)
    {

    }

    ~Bus()=default;
    virtual Bus* clone() const override
    {
        return new Bus(*this);
    }
    void settk(int nr)
    {
        tkprice=nr;
    }
    void setpass(int nr)
    {
        passnr=nr;
    }
    int gettk()
    {
        return tkprice;
    }
    int getpass()
    {
        return passnr;
    }
    virtual Bus* clone()
    {
        return new Bus(*this);
    };
    void print(ostream& os) const override
    {
    os<<"AUTOCAR"<<endl;
    Car::print(os);
    os<<"PRET BILET: "<<tkprice<<endl;
    os<<"NUMAR PASAGERI: "<<passnr<<endl;
    }
    friend ostream& operator<<(ostream &os, const Bus& b)
    {
        b.print(os);
        os<<"----------------------------------------------------------"<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, Bus& n) {
        is>>static_cast<Car&> (n);
        cout<<"Introduceti numar pasageri: ";
        is>>n.passnr;
        cout<<endl;
        cout<<"Introduceti pret bilet: ";
        is>>n.tkprice;
        cout<<endl;
        return is;

    }
}
;
class eroare : public exception
{
public:
    eroare()=default;
    const char* what() const noexcept override
    {
        return "The car with this id doesn't exist.";
    };
};



class Garage
{
    int carnr;
    static vector <shared_ptr<Car>> c;
public:
    Garage()
    {
        carnr=0;
    }
    void addcar(shared_ptr<Car> car)
    {
        c.push_back(car);
        carnr++;
    }
    void printcar()
    {
        for(auto &Car: c)
        {
            cout<<*Car<<endl;
        }
    }
    shared_ptr<Car> findcar(int n)
    {
        for(auto &Car: c)
        {
            if(Car->getID()==n)
                return Car;
        }
        throw eroare();
    }
};
vector<shared_ptr<Car>> Garage::c;

template<class T1, class T2>
void getMax(T1 a,T2 b)
{

    if(a>b)
    {
        cout<<a;
    }
    else
        cout<<b;
}
template<>
void getMax(shared_ptr<Car> a,shared_ptr<Car> b)
{

    cout<<endl<<"Are portbagajul mai mare:"<<endl;
    if(*a>*b)
    {
        cout<<*a;
    }
    else
        cout<<*b;
}


int main() {

Garage g;


///---------------------------------------MENIU------------------------------------

try {
    cout<<*g.findcar(4);
}
catch (const eroare& err)
{
    cout<<endl<<err.what()<<endl;
}

    int i=4;
    while(i!=100)
    {
        cout<<"-----------------------------------------------------------"<<endl;
        cout<<"CE DORITI SA FACETI?"<<endl;
        cout<<"-----------------------------------------------------------"<<endl;
        cout<<"0.Iesire"<<endl;
        cout<<"1.Adaugare n masini"<<endl;
        cout<<"2.Adaugare n autocare"<<endl;
        cout<<"3.Cautare masina/autocar dupa id si afisare"<<endl;
        cout<<"4.Comparare marime portbagaj 2 masini/autocare"<<endl;
        cout<<"5.Afisare masini din garaj"<<endl;
        cout<<"6.Adaugare masina pre-construita"<<endl;
        cout<<"7.Construire si adaugare masina"<<endl;
        cout<<endl<<"-----------------------------------------------------------"<<endl;
        cout<<"Introduceti numarul actiunii dorite:";
        cin>>i;
        cout<<endl<<"-----------------------------------------------------------"<<endl;

        if(i==1)
        {
            int nr;
            cout<<"Introduceti numarul de masini: ";
            cin>>nr;
            cout<<"-----------------------------------------------------------"<<endl;
            for (int j = 0; j < nr; j++) {
                auto m = std::make_shared<Car>();
                cin >> *m;
                g.addcar(m);
            }

        }
        if(i==2)
        {
            int nr;
            cout<<"Introduceti numarul de autocare: ";
            cin>>nr;
            cout<<"-----------------------------------------------------------"<<endl;
            for (int j = 0; j < nr; j++) {
                auto m = std::make_shared<Bus>();
                cin >> *m;
                g.addcar(m);
            }
        }
        if(i==3)
        {
            int s;
            cout<<"Introduceti id:";
            cin>>s;
            cout<<"-----------------------------------------------------------"<<endl;

            try {
                cout << *g.findcar(s);
            }
            catch(const eroare& err)
            {
                cout<<endl<<err.what()<<endl;
            }
        }
        if(i==4)
        {
            try{
                int s1,s2;
                cout<<"Introduceti id-ul vehicul 1:";
                cin>>s1;
                cout<<"-----------------------------------------------------------"<<endl;
                g.findcar(s1);
                cout<<"Introduceti id-ul vehicul 2:";
                cin>>s2;
                cout<<"-----------------------------------------------------------"<<endl;
                g.findcar(s2);
                getMax(g.findcar(s1),g.findcar(s2));
            }
            catch(const eroare& err)
            {
                cout<<endl<<err.what()<<endl;
            }
        }
        if(i==5)
        {
            g.printcar();
        }
        if(i==6) {
            int j;
            cout << "-----------------------------------------------------------" << endl;
            cout << "CE MASINA DORITI SA ADAUGATI?" << endl;
            cout << "-----------------------------------------------------------" << endl;
            cout << "0.Anulare" << endl;
            cout << "1.Adaugare BMW" << endl;
            cout << "2.Adaugare AUDI" << endl;
            cout << "3.Adaugare CITROEN" << endl;
            cout << endl << "-----------------------------------------------------------" << endl;
            cout << "Introduceti numarul masinii dorite:";
            cin >> j;
            cout << endl << "-----------------------------------------------------------" << endl;
            if (j == 1)
            {
                auto m = std::make_shared<Car>();
                *m=CarFactory::BMW();
                g.addcar(m);
            }
            if(j==2)
            {
                auto m = std::make_shared<Car>();
                *m=CarFactory::AUDI();
                g.addcar(m);
            }
            if(j==3)
            {
                auto m = std::make_shared<Car>();
                *m=CarFactory::CITROEN();
                g.addcar(m);
            }
        }
        if(i==7)
        {
            int snr,dnr,tsize;
            string clr;
            CarBuilder b;
            cout << "Introduceti numar locuri: ";
            cin >> snr;
            cout << endl;
            cout << "Introduceti numar usi: ";
            cin >> dnr;
            cout << endl;
            cout << "Introduceti marimea portbagajului: ";
            cin >> tsize;
            cout << endl;
            cout << "Introduceti culoarea:";
            cin >> clr;
            cout << endl;
            auto m = std::make_shared<Car>();
            *m=b.seatsnr(snr).doorsnr(dnr).trunksize(tsize).colour(clr).id().build();
            g.addcar(m);

        }

        if(i==0)
            i=100;



    }

    return 0;
}
