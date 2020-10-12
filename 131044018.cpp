#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<limits.h>


using namespace std;

class ConnectFour{

     private:
        class Cell{
            public:
                Cell();
                void setCell(char newCell){ cell=newCell;}
                void setRow(int newRow){ row=newRow;}
                void setValue(char newValue) {value=newValue;}
                char getValue() const {return value;}
                char getCell() const {return cell;}
                int getRow() const {return row;}
                /*Stream Insertion and Stream Extraction Operators Overloads*/
                friend ostream& operator <<(ostream& output,const Cell& c){
                    output<< c.cell<<" " <<c.row << c.value<<endl;
                    return output;
                }
                friend istream& operator >>(istream& input,Cell& c){

                    input>>c.value;
                    return input;
                }
                bool operator ==(const Cell &) const;

            private:
                char cell;
                int row;
                char value;/*Value can be "X","O"(if game ended it may be lover case) or '.'*/


        };
        static int livingCell;
        int height;
        int width;
        int moveCounter=0;
        char column;
        string filename;
        string input;
        string inputCommand;
        int tempSize;
        int lastMove[1][2];
        bool finished=false;
        /**Checker functions**/
        bool checkColumn() const;
        bool checkRow() const;
        bool isSizeLegal(const int height,const int width) const{return (height<=0||width<=0);}
        /***End of checkers***/
        /**Play Funcitons**/
        void addMoveToBoard();
        void play();
        void play(char choice);
        void undoMove();
        void transformBoard();
        /**End of Play Functions**/
        /**Save And Read Functions**/
        void read();
        void write() const;
        Cell ** gameBoard;
        /*********************/

    public:

        ConnectFour();
        ConnectFour(int);
        ConnectFour(int,int);
        ConnectFour(const ConnectFour&);
        ConnectFour(string);
        ~ConnectFour(){for(int i=0;i<height;++i)delete[] gameBoard[i];delete[] gameBoard;}
        ConnectFour & operator =(const ConnectFour &);
        bool isFinished() const;//++
        void setGameCells();//++
        void printGame() const;//++
        void playGame();
        int getCurrentHeight() const {return height;}
        int getCurrentWidth() const {return width;}
        bool getFinished() const {return finished;}
        int getMoveCounter() const {return moveCounter;}
        static int getLivingCell();
        bool compare(const ConnectFour&) const;

};
int ConnectFour::livingCell = 0;
int ConnectFour::getLivingCell(){
    return livingCell;
}
/***************Constructors   ***************/
ConnectFour::ConnectFour(){
    cout<<"If you want to load a game write LOAD "<<endl;
    cout<<"If you don't want to load a game press any key and enter "<<endl;
    cin>>input;
    if(input=="LOAD"){
        cout<<"Please enter the filename"<<endl;
        cin>>filename;
        read();
        }
    else{
    height=5;
    width=5;
    setGameCells();
}
}
ConnectFour::ConnectFour(int newHeight):ConnectFour(){/*This constructor used for vector push_back*/ }
ConnectFour::ConnectFour(int newHeight,int newWidth) : height(newHeight),width(newWidth) {
    if(isSizeLegal(newHeight,newWidth)){
        cerr<<"Wrond size ! Size must be bigger than 0 !"<<endl;
        ConnectFour();
    }
    setGameCells();
}
ConnectFour::ConnectFour(string file){
    input = file;
    if(input=="LOAD"){
        cout<<"Please enter the filename"<<endl;
        cin>>filename;
        read();
        }else{
            ifstream inputFile;
            inputFile.open(file.c_str());
           int newWHeight=1;
           string line;
           getline(inputFile,line);
           width=line.size();
           while(getline(inputFile,line))
                ++newWHeight;
           height=newWHeight;
            inputFile.clear();
            inputFile.seekg(0, ios::beg);
            setGameCells();
            /*burada dosyadaki board atanacak!!!*/
            for(int i = 0;i<height;++i){
                getline(inputFile,line);
                for(int j=0;j<width;++j){
                    if(line[j]==' ')
                        gameBoard[i][j].setValue(' ');
                    else if(line[j]=='*')
                        gameBoard[i][j].setValue('.');

                }

            }

            inputFile.close();
        }


}
ConnectFour::ConnectFour(const ConnectFour & other){
    height=other.height;
    width = other.width;
    moveCounter=other.moveCounter;
    column=other.column;
    filename=other.filename;
    input=other.input;
    inputCommand=other.inputCommand;
    tempSize=other.tempSize;
    lastMove[0][0]=other.lastMove[0][0];
    lastMove[0][1]=other.lastMove[0][1];
    finished = other.finished;
    for(int i=0;i<height;++i)
        delete[] gameBoard[i];
    delete[] gameBoard;
    gameBoard = new Cell*[height];
    for(int i=0;i<height;++i)
        gameBoard[i] = new Cell[width];
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j)
            gameBoard[i][j]=other.gameBoard[i][j];
    }

}
/****************** Operator Overloads ************************/
ConnectFour& ConnectFour::operator=(const ConnectFour& other){

    if(this==&other)
        return *this;
    height=other.height;
    width = other.width;
    moveCounter=other.moveCounter;
    column=other.column;
    filename=other.filename;
    input=other.input;
    inputCommand=other.inputCommand;
    tempSize=other.tempSize;
    lastMove[0][0]=other.lastMove[0][0];
    lastMove[0][1]=other.lastMove[0][1];
    finished = other.finished;
    for(int i=0;i<height;++i)
        delete[] gameBoard[i];
    delete[] gameBoard;
    gameBoard = new Cell*[height];
    for(int i=0;i<height;++i)
        gameBoard[i] = new Cell[width];
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j)
            gameBoard[i][j]=other.gameBoard[i][j];
    }

}
bool ConnectFour::Cell::operator ==(const Cell & other) const{

     return(cell==other.cell&&value==other.value&&row==other.row);

}
/***********************ConnectFour Functions**************************/
void ConnectFour::write() const{
    //string filename="file.txt";
    ofstream file;
    file.open(filename.c_str(),ios::out);
    file<<inputCommand[0]<<endl;
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            file<<gameBoard[i][j].getValue();
        }

        file<<endl;
    }
    cout<<"file succesfully saved"<<endl;
file.close();
}
void ConnectFour::read(){
    //string filename="file.txt";
    string line;
    ifstream file;
    int updateMVCounter=1;
    int updateLivingCell=0;

    int j=0;
    file.open(filename.c_str());
    getline(file,line);
    inputCommand[0]=line[0];
    getline(file,line);
    int loadedWidth=line.size();/*This is updates width*/
    ++j;
    while(getline(file,line)){
        /*Finding height*/
    ++j;
    }
    height=j;
    /*Resizing board*/
    gameBoard=new Cell*[j];
    for(int i=0;i<j;++i){
        gameBoard[i]=new Cell[loadedWidth];
    }
    file.clear();
    file.seekg(0, ios::beg);
    j=0;
    getline(file,line);
    char b;
    /*Set board*/
    while(getline(file,line)){
        for(int i=0;i<loadedWidth;++i){
            b=line[i];
            gameBoard[j][i].setValue(b);
            gameBoard[j][i].setCell(i+65);
            gameBoard[j][i].setRow(j);
            if(b=='X'||b=='O'){
                ++updateMVCounter;
                ++updateLivingCell;
            }
        }
      ++j;
    }
    width=loadedWidth;
    livingCell+=updateLivingCell;
    moveCounter=updateMVCounter;
}
void ConnectFour::setGameCells(){
    gameBoard = new Cell*[height];
    for(int i=0;i<height;++i){
        gameBoard[i]=new Cell[width];
    }
    for(int i=0;i<width;++i){
        for(int j=0;j<height;++j){
            gameBoard[j][i].setCell(i+65);
            gameBoard[j][i].setRow(j+1);
        }
    }

}
void ConnectFour::printGame() const{

for(int i=0;i<width;++i)
        cout<<gameBoard[0][i].getCell();
    cout<<endl;
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            cout<<gameBoard[i][j].getValue();
        }
        cout<<endl;
    }

    cout<<endl;

}

ConnectFour::Cell::Cell() : value('.') {     }
/*Check Function*/

bool ConnectFour::checkRow() const{
    for(int i=0;i<height;++i){
        for(int j=0;j<width;++j){
            if(gameBoard[i][j].getValue()=='X'&&gameBoard[i][j+1].getValue()=='X'&&
               gameBoard[i][j+2].getValue()=='X'&&gameBoard[i][j+3].getValue()=='X'){
                   // cout<<"PLAYER 1 WON"<<endl;
                    return true;
                    }
            else if(gameBoard[i][j].getValue()=='O'&&gameBoard[i][j+1].getValue()=='O'&&
                    gameBoard[i][j+2].getValue()=='O'&&gameBoard[i][j+3].getValue()=='O'){
                    //cout<<"PLAYER 2 WON"<<endl;
                    return true;
                    }
        }
    }
    return false;
}

bool ConnectFour::checkColumn()const{
    for(int i=0;i<width;++i){
        for(int j=0;j<height-3;++j){
            if(gameBoard[j][i].getValue()=='X'&&gameBoard[j+1][i].getValue()=='X'&&
               gameBoard[j+2][i].getValue()=='X'&&gameBoard[j+3][i].getValue()=='X'){
                    //cout<<"PLAYER 1 WON"<<endl;
                    return true;
                    }
            else if(gameBoard[j][i].getValue()=='O'&&gameBoard[j+1][i].getValue()=='O'&&
                    gameBoard[j+2][i].getValue()=='O'&&gameBoard[j+3][i].getValue()=='O'){
                    //cout<<"PLAYER 2 WON"<<endl;
                    return true;
                    }
        }
    }
    return false;
}

bool ConnectFour::isFinished() const{

    if(moveCounter==height*width)
        return true;

    if(checkRow()||checkColumn())
        return true;

    for(int i=0;i<height-3;++i){
        for(int j=0;j<width-3;++j){
            if(gameBoard[i][j].getValue()=='X'&&gameBoard[i+1][j+1].getValue()=='X'
                &&gameBoard[i+2][j+2].getValue()=='X'&&gameBoard[i+3][j+3].getValue()=='X'){
                    //cout<<"PLAYER 1 WON"<<endl;
                    return true;
                    }
            else if(gameBoard[i][j].getValue()=='O'&&gameBoard[i+1][j+1].getValue()=='O'
                    &&gameBoard[i+2][j+2].getValue()=='O'&&gameBoard[i+3][j+3].getValue()=='O'){
                        //cout<<"PLAYER 2 WON"<<endl;
                        return true;
                        }
            }
        }

    for(int i=0;i<height-3;++i){
        for(int j=3;j<width;++j){
            if(gameBoard[i][j].getValue()=='X'&&gameBoard[i+1][j-1].getValue()=='X'
                &&gameBoard[i+2][j-2].getValue()=='X'&&gameBoard[i+3][j-3].getValue()=='X'){
                    //cout<<"PLAYER 1 WON"<<endl;
                    return true;
                    }
            else if(gameBoard[i][j].getValue()=='O'&&gameBoard[i+1][j-1].getValue()=='O'
                    &&gameBoard[i+2][j-2].getValue()=='O'&&gameBoard[i+3][j-3].getValue()=='O'){
                        //cout<<"PLAYER 2 WON"<<endl;
                        return true;
                        }
            }
        }

    return false;

}

void ConnectFour::addMoveToBoard(){


    int i=0;
    int k = column-'A';
    char a;
    if(moveCounter%2==0)
        a='O';
    else
        a='X';
    while(i<height){
        if(gameBoard[i][k].getValue()!='.'&&gameBoard[i][k].getValue()!=' '){
            gameBoard[i-1][k].setValue(a);
            lastMove[0][0]=i-1;
            lastMove[0][1]=k;
            i=height+1;
        }else if(i==height-1&&gameBoard[i][k].getValue()=='.'){
            gameBoard[i][k].setValue(a);
            lastMove[0][0]=i;
            lastMove[0][1]=k;
            i=height+1;
	}
	++i;
    }
    ++moveCounter;
    ++livingCell;
}
/******CPU Play Function ******/
/*This function used for CPU play*/
void ConnectFour::undoMove(){
    gameBoard[lastMove[0][0]][lastMove[0][1]].setValue('.');
    --moveCounter;
    --livingCell;
    /*!!!!Static degisken burada azaltilacak !!!!*/
}

void ConnectFour::play(){
        cout<<"Enter the move(If you enter SAVE board will be saved) : "<<endl;
        cin>>input;
        if(input=="SAVE"){
            cout<<"Game Succesfully SAVED. Please enter the filename"<<endl;
            cin>>filename;
            write();
            cout<<"Game Succesfully SAVED. Please enter move"<<endl;
            cin>>input;
        }
        while(input=="SAVE"){
            cout<<"You already saved game please enter move"<<endl;
            cin>>input;
        }
        column=toupper(input[0]);
        while(column<'A'||column>'A'+width){
            cout<<"You entered invalid move please enter valid move : "<<endl;
            cin>>input;
            column=toupper(input[0]);
        }
        addMoveToBoard();
        cout<<"livingCell: "<<ConnectFour::getLivingCell()<<endl;
        printGame();
        if(isFinished()){
            cout<<"GAME OVER"<<endl;
            moveCounter=1;
        }else{

        int index=0,index2=0;
        int i = 0,j=0;
        int priorityMove;
        bool winningMove=false;
        /*Checks if cpu can finish*/
        while(i<width){
            column=i+65;
            addMoveToBoard();
            if(isFinished()&&moveCounter%2==0){
                    index=i;
                    i=width+5;
                    priorityMove=1;
                    winningMove=true;
            }

            undoMove();
        ++i;
        }
        /*Checks is user finish next move and try to prevent it*/
        ++moveCounter;
        while(j<width){
            column=j+65;
            addMoveToBoard();

            if(isFinished()&&moveCounter%2==0){
                    index2=j;
                    j=width+5;
                    priorityMove=2;
            }
        undoMove();
        ++j;

        }
        --moveCounter;
        if(priorityMove==1||winningMove){
            column = index+65;
            addMoveToBoard();
        }else if(priorityMove==2){
            column = index2+65;
            addMoveToBoard();
        }else{
        //Random move according to moveCount
        column=(moveCounter%width)+65;
        addMoveToBoard();
        }
        cout<<"livingCell: "<<ConnectFour::getLivingCell()<<endl;
        printGame();
        if(isFinished()){
        cout<<"GAME OVER"<<endl;
        moveCounter=2;
        }
        }


}

/*******End of CPU plays*********/

/*User to User Play*/
void ConnectFour::play(char a){
    cout<<"Enter the move : "<<endl;
    cin>>input;
     if(input=="SAVE"){
            cout<<"Please enter the filename"<<endl;
            cin>>filename;
            write();
            cout<<"Game Succesfuly saved. Please enter move"<<endl;
            cin>>input;
        }
        while(input=="SAVE"){
            cout<<"You already saved game please enter move"<<endl;
            cin>>input;
    }
    column=toupper(input[0]);
    while(column<'A'||column>'A'+width){
    cout<<"You entered invalid move please enter valid move : "<<endl;
    cin>>input;
    column=toupper(input[0]);
    }
    addMoveToBoard();
    cout<<"livingCell: "<<ConnectFour::getLivingCell()<<endl;
    printGame();
    if(isFinished()){
            cout<<"GAME OVER"<<endl;
            moveCounter=1;
    }else{
    cout<<"Enter the move : "<<endl;
    cin>>input;
     if(input=="SAVE"){
            cout<<"Please enter the filename"<<endl;
            cin>>filename;
            write();
            cout<<"Game Succesfuly saved. Please enter move"<<endl;
            cin>>input;
        }
        while(input=="SAVE"){
            cout<<"You already saved game please enter move"<<endl;
            cin>>input;
        }
    column=toupper(input[0]);
    while(column<'A'||column>'A'+width){
    cout<<"You entered invalid move please enter valid move : "<<endl;
    cin>>input;
    column=toupper(input[0]);
    }
    addMoveToBoard();
    cout<<"livingCell: "<<ConnectFour::getLivingCell()<<endl;
    printGame();
    if(isFinished()){
        cout<<"GAME OVER"<<endl;
        moveCounter=2;
    }
    }
}
void ConnectFour::playGame(){
    if(moveCounter==0){
    cout<<"If  you want to play user vs user enter P "
        <<"If you want to play user vs cpu enter C "<<endl;
    cin>>inputCommand;
    bool inputTrue = inputCommand.size()<2||inputCommand.size()>0
          &&inputCommand[0]=='C'||inputCommand[0]=='P'||inputCommand[0]=='c'||inputCommand[0]=='p';
    while(!inputTrue){
        cerr<<"Wrong input !! Please correct your choice "<<endl;
        cin>>inputCommand;
    }
    ++moveCounter;
    }else{
    if(inputCommand[0]=='P'||inputCommand[0]=='p'&&cin.peek()!=cin.eof())
        play('p');
    else if(inputCommand[0]=='c'||inputCommand[0]=='C'&&cin.peek()!=cin.eof())
        play();
    }
}
bool ConnectFour::compare(const ConnectFour& other)const {
    ConnectFour a(height,width),b;
    /*Creates temporary variables.So function doesn't change neither this object nor other object*/
    b=other;
    int i=0;
    while(i<width){
        a.column=i+65;
        a.addMoveToBoard();
        if(a.isFinished()&&a.moveCounter%2==1)
                return true;

        a.undoMove();
        ++i;
        }
        i=0;
    while(i<b.width){
        b.column=i+65;
        b.addMoveToBoard();
        if(b.isFinished()&&b.moveCounter%2==1)
            return false;
        b.undoMove();
        ++i;
    }
    return true;
}
int main(){
    int obj=0;
    int i = 0;
    string gameMode;
    bool isGameEnd = false;
    cout<<"WELCOME TO CONNECTFOUR GAME !!! "<<endl;
    cout<<"Please choose the gamemode(S or M)"<<endl;
    cin>>gameMode;
    bool exp=gameMode.size()==1&&gameMode[0]=='S'||gameMode[0]=='M'||gameMode[0]=='s'||gameMode[0]=='m';
    while(!exp){
        cout<<"Wrong input please enter correct input.You must choose S(single) or M(multi): "<<endl;
        cin>>gameMode;
        exp=gameMode.size()==1&&gameMode[0]=='S'||gameMode[0]=='M'||gameMode[0]=='s'||gameMode[0]=='m';
    }
    if(gameMode[0]=='S'||gameMode[0]=='s'){
        string inp;
        cin>>inp;
        ConnectFour single(inp);
        single.printGame();

        while(!isGameEnd){
            isGameEnd=single.isFinished();
            single.playGame();
            if(cin.peek()==cin.eof())
            isGameEnd=true;

        }
        if(single.getMoveCounter()%2==1)
            cout<<"WINNER IS PLAYER 1"<<endl;
        else
            cout<<"WINNER IS PLAYER 2"<<endl;
    }else{
    vector<ConnectFour> multi;
    for(int i=1;i<6;++i){
        string inp;
        cin>>inp;
        cout<<"Please enter OBJECT "<<i<<" data"<<endl;
        multi.push_back(inp);
    }
    isGameEnd = false;
        cout<<"Please enter the object"<<endl;
        cin>>obj;
    while(!isGameEnd){
        cout<<"You are currently playing obj : "<<i<<endl;
        multi[obj].printGame();
        multi[obj].playGame();
        cout<<ConnectFour::getLivingCell<<endl;
        cout<<"Please enter the object"<<endl;
        cin>>obj;
        isGameEnd = multi[0].getFinished()&&multi[1].getFinished()
                    &&multi[2].getFinished()&&multi[3].getFinished()&&multi[4].getFinished();
        if(cin.peek()==cin.eof())
            isGameEnd=true;
    }

    }
    return 0;

}

