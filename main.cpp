#include <iostream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <unordered_map>
#include <queue>

std::vector<std::string> GetNumberSlices(std::string Number)
{
    std::vector<std::string> NumSlices; 
    std::string slice;
     int SliceLens[] = {2, 3, 7} ;

    for (int i = 0; i < 3; i++) // очень некрасивый цикл, но работает.
    {
    
        for (int j = 0; j < SliceLens[i]; j++)
        {
            if(i == 1){
                slice.push_back(Number.operator[](i+j+1));
            }
            if(i == 2){
                slice.push_back(Number.operator[](i+j+3));
            }
            
            if(i == 0){
                slice.push_back(Number.operator[](i+j));
            }  
        }

        NumSlices.push_back(slice);
        slice.clear();  
    }
    return NumSlices;
}
////////////////////////////////////

///////////////////
// User 
///////////////////

class User {
    private:
    std::queue<std::string> MsgBuf_;
    std::string UserCode_;


    public:
        void                    SetMsgBuf(std::string msg)      {MsgBuf_.push(msg);};
        std::queue<std::string> GetMsgBuf()                     {return MsgBuf_;}
        void                    SetUserCode(std::string UsrCode){UserCode_ = UsrCode;}
        std::string             GetUserCode()                   {return UserCode_;}

        void PushMsg(std::string msg) {
            if(msg.length() == 0){
                std::cout << "Empty msg is not allowed";
            }
            else{
                MsgBuf_.push(msg);
            }   
        };

        void PrintMsgLast() {
            if (MsgBuf_.size() == 0){
                std::cout << "There is no any messages" << std::endl;
                return;
            }
            else {
                 
                std::cout << MsgBuf_.front() << std::endl;

                MsgBuf_.pop();
            }
            return;  
        };      
}; 

///////////////////////////////////////////////////

/////////////////////////////////////
// Operator
///////////////////////////////////////

class Operator {

    private:
        std::string OperatorCode_;
        std::string CountryCode_;

        std::unordered_map<std::string, User*>       Users;
    public:
        std::string GetOperatorCode(){return OperatorCode_;};
        std::string GetCountryCode (){return CountryCode_; };
        void        SetOperatorCode(std::string OperatorCode){OperatorCode_ = OperatorCode;};
        void        SetCountryCode (std::string CountryCode ){CountryCode_  = CountryCode; }; 
        User*       GetUser        (std::string UserCode)    {return Users[UserCode];}
        void        SetUserMsg     (std::string UsrCode, std::string Msg) {Users[UsrCode]->PushMsg(Msg);}
        void GetUserMsg            (std::string Usrcode)                  {Users[Usrcode]->PrintMsgLast();}    
        ~Operator() {
            std::unordered_map<std::string, User*>::iterator it = Users.begin();
            while (it != Users.end()) {
                delete it->second;
                Users.erase(it);
                it++;
            }
        };

        User* BuildUser(std::string UsrNum) {
            if(IsUserExist(UsrNum))
            {
                std::cout << "This user already exists" << std::endl;
                return Users[UsrNum];
            }
            
            else
            {
                User* NewUsr = new User;
                NewUsr->SetUserCode(UsrNum);
                Users[UsrNum] = NewUsr;
                std::cout << "New user builded" << std::endl;
                return NewUsr;
            }  
        };

        bool IsValidNum (std::string Num) {
            std::vector <std::string> Slices = GetNumberSlices(Num);
            if (Slices[1] != OperatorCode_)
            {

                std::cout << "It's another operator. Op Code:" << OperatorCode_ << " slices[1]:" << Slices[1] << " Num" << Num << std::endl;
                return false;
            }  
            else {return true;}
        }

        bool IsUserExist(std::string UsrNum) {
            if (Users.find(UsrNum) != Users.end())
            {
                return true;
            }

            else 
            {
                return false;
            }
            
        }

        std::unordered_map<std::string, User*>::iterator SeekUser(std::string UsrNum){
            return Users.find(UsrNum);
        }
        std::unordered_map<std::string, User*>::iterator SeekEnd(){
            return Users.end();
        }

};

///////////////////////////////////////

/////////////////////////////////
// Country 
//////////////////////////////////

class Country {
    private: 
        std::string CountryCode_;
        std::unordered_map<std::string, Operator*>   Operators;
    public:
        std::string GetContryCode()                                     {return CountryCode_;};
        void        SetContryCode(std::string CountryCode)              {CountryCode_= CountryCode;}
        void        SetOperatorMsg(std::string OpCode, std::string UsrCode, std::string Msg) {Operators[OpCode]->SetUserMsg(UsrCode , Msg);}
        void        GetOperatorMsg(std::string OpCode, std::string UsrCode)                  { Operators[OpCode]->GetUserMsg(UsrCode);}
        ~Country() {
            std::unordered_map<std::string, Operator*>::iterator it = Operators.begin();
            while (it != Operators.end()) {
                delete it->second;

                Operators.erase(it);
                it++;
            }
        };

        Operator* BuildOperator(std::string OperatorCode){
             if (Operators.find(OperatorCode) != Operators.end()){
                std::cout << "country is already exists" << std::endl;

                return Operators[OperatorCode];
            }
            else {
                Operator* nOperator = new Operator;
                Operators[OperatorCode] = nOperator;

                nOperator->SetCountryCode(CountryCode_);
                nOperator->SetOperatorCode(OperatorCode);
            
                return nOperator;
            }
        };

        std::unordered_map<std::string, Operator*>::iterator SeeckOperator(std::string OperatorCode){
            return this->Operators.find(OperatorCode);
        }
        std::unordered_map<std::string, Operator*>::iterator SeeckEnd(){
            return this->Operators.end();
        }
        Operator* GetOperator(std::string OpCode){
            return this->Operators[OpCode];
        }
};

////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Switch
/////////////////////////////////////////////////////////////


class Switch {
    private:
        std::unordered_map<std::string, Country*> Countries;

public:
    void SendMsg(std::string UsrNum, std::string Msg) {
        auto NumSlices = GetNumberSlices(UsrNum);
        
        auto It1 = this->Countries.find(NumSlices[0]);
        if(It1 == this->Countries.end()){
            std::cout << "This country does not exist" << std::endl;
        }
        else{
            auto It2 = this->Countries[NumSlices[0]]->SeeckOperator(NumSlices[1]);
            if (It2 == this->Countries[NumSlices[0]]->SeeckEnd()){
                std::cout << "This operator does not exist" <<std::endl;
            }
            else{
                auto Op = this->Countries[NumSlices[0]]->GetOperator(NumSlices[1]);
                auto It3 = Op->SeekUser(NumSlices[2]);

                if (It3 == Op->SeekEnd()){
                    std::cout << "This user does not exist" <<std::endl;
                }
                else{
                    auto Usr = Op->GetUser(NumSlices[2]);
                    this->Countries[NumSlices[0]]->SetOperatorMsg(NumSlices[1], NumSlices[2], "Hello");
                }
            }
        }
    };
        
    void ReadMsg(std::string UsrNum) {
        auto NumSlices = GetNumberSlices(UsrNum);
        this->Countries[NumSlices[0]]->GetOperatorMsg(NumSlices[1], NumSlices[2]);  
    };

    Country* BuildCountry(std::string CountryCode) {
        if (Countries.find(CountryCode) != Countries.end()){
            std::cout << "country is already exists" << std::endl;

            return Countries[CountryCode];
        }
        else {
            Country* nCountry = new Country;
            Countries[CountryCode] = nCountry;
            nCountry->SetContryCode(CountryCode);

            return nCountry;
        }  
    };
};

int main()
{
    Switch x1;
    Country* Russia = x1.BuildCountry("+7");
    Operator* MTS = Russia->BuildOperator("987");
    User* Iam = MTS->BuildUser("1695504");

    x1.SendMsg("+79871695504", "Hello");
    x1.ReadMsg("+79871695504");
    x1.ReadMsg("+79871695504");

    return 0;
}

