

#define MAX_ROLL_IN_FILE 32 // value of "L"
#define ROLL_COUNT_SIZE 2 // size of the chars in no of roll, char len of "L"
#define ROLL_LEN 7 // length of each roll no
#define HEAP_FILE_NAME_LEN 7
#define BST_MAX_LEN 5 // string length of (maximum no of nodes in BST)
#define INF 10000000 // Infinite
#define CHILD_NODE 'c'
#define INTERNAL_NODE 'i'


void showMenu();


#include <iostream>
#include <fstream>

using namespace std;


// Helpers...
/*
********* stores num as char in argument "c" **********
*@param c "the array where to write"
*@param num "the number to be converted"
*@param c_len "length of array pointed by c"
*/
void num2arr( char* c, int num, int c_len) {
    for(int i=0; i<c_len; i++) {
        c[c_len- i- 1]= num%10 + 48;
        num/= 10;
    }
}

/*
********* return starting position of a "roll no" **********
*************** in the file stored as heap  ****************
*@param index "index to be cnverted"
*@param roll_len "length of a roll no"
*/
int fileIndexOf(int index) {
    int res= ROLL_COUNT_SIZE+1;
    res+= (ROLL_LEN+1)*(index+1);
    return res;
}


/********************* Create each heap file **********************
*********** Files will have length and max Roll at begining********
******************************************************************/

class Heaper {
private:
    fstream file;
public:
    Heaper(string _file_name) {
        file.open(_file_name);
        // sometime, the file was not creating, so Quick FIX
        if(!file) {
            file.open(_file_name, ios::out);
            file.close();
            file.open(_file_name, ios::out | ios::in);
            // cout << "\n\tCreated new File\n\n";
        }
        if(!file) {
            cout << "\n\t\aerrror opening file\n";
            exit(-1);
        }
    }
    ~Heaper() {
        if(file) {
            // cout << "\n\tExitting\n";
            file.flush();
            file.close();
        }
    }

    void init() {
        writeLength(0);
        updateMax(0);
        for(int i=0; i<MAX_ROLL_IN_FILE; i++) {
            writeRollAt(i, 0);
        }
    }

    void writeLength(int length) {
        char arr[ROLL_COUNT_SIZE+1];
        for(int i=0; i<ROLL_COUNT_SIZE; i++) {
            arr[ROLL_COUNT_SIZE-i-1]= length%10 + 48;
            length= length/10;
        }
        arr[ROLL_COUNT_SIZE]= '\n';
        file.seekg(0);
        file.write(arr, ROLL_COUNT_SIZE+1);
        file.flush();
    }

    void updateMax(int max) {
        char max_str[ROLL_LEN+1];
        max_str[ROLL_LEN]= '\n';
        num2arr(max_str, max, ROLL_LEN);
        file.seekg(fileIndexOf(-1)); // index -1= 3 is position of max_roll
        file.write(max_str, ROLL_LEN+1);
        file.flush();
    }

    void writeRollAt(int index, int roll_no) {
        char roll_arr[ROLL_LEN+1];
        roll_arr[ROLL_LEN]= ' ';
        if(index>-1 && index<=MAX_ROLL_IN_FILE) {
            int file_index= fileIndexOf(index);
            num2arr(roll_arr, roll_no, ROLL_LEN);
            file.seekg(file_index);
            file.write(roll_arr, ROLL_LEN+1);
            file.flush();
        }   
    }

    int search(int roll_no) {
        int size= readLength();
        for(int i=0; i<size; i++) {
            int tmp_roll= readRollFrom(i);
            if(tmp_roll == roll_no) {
                return true;
            }
        }
        return false;
    }

    int readLength() {
        int res= 0;
        char c;
        int base= 0;
        for(int i=0; i<ROLL_COUNT_SIZE; i++) {
            file.seekg(base+i);
            res *= 10;
            file.get(c);
            res+= c-48;
        }
        return res;
    }

    int readRollFrom(int index) {
        if(index == INF) {
            return INF;
        }
        if(index < -1 || index > MAX_ROLL_IN_FILE) {
            return 0;
        }
        int res= 0;
        char c;
        file.seekg(fileIndexOf(index));
        for(int i=0; i<ROLL_LEN; i++) {
            res *= 10;
            file.get(c);
            int _c= c;
            res+= c-48;
        }
        return res;
    }

    int readMaxRoll() {
        return readRollFrom(-1);
    }

    void writeMaxRoll(int roll_no) {
        char roll_arr[ROLL_LEN+1];
        roll_arr[ROLL_LEN]= '\n';
        num2arr(roll_arr, roll_no, ROLL_LEN);
        file.seekg(fileIndexOf(-1));
        file.write(roll_arr, ROLL_LEN+1);
        file.flush();
    }

    int insert(int roll_no) {
        int cur_length= readLength();
        int max_roll= readMaxRoll();
        if(cur_length >= MAX_ROLL_IN_FILE) {
            cout << "\n\tError on insert\n\n";
            exit(-1);
        }
        writeRollAt(cur_length, roll_no);
        writeLength(cur_length+1);
        if(max_roll< roll_no) {
            writeMaxRoll(roll_no);
        }
        heapify_up(cur_length);
    }


/*
                (0)
               /  \
            (1)    (2)
           /  \    /  \
         (3)  (4)(5)  (6)
*/

    void heapify_up(int index) {
        int me_index, bro_index, dad_index;
        if(index % 2 == 0) {
            me_index= index;
            bro_index= index-1;
            dad_index= index/2 -1;
        } else {
            me_index= index;
            bro_index= INF;
            dad_index= (index-1)/2;
        }
        int me= readRollFrom(me_index);
        int bro= readRollFrom(bro_index);
        int dad= readRollFrom(dad_index);
        bool swapped= false;
        if(me<dad) {
            writeRollAt(me_index, dad);
            writeRollAt(dad_index, me);
            swapped= true;
        } else if(bro < dad) {
            writeRollAt(bro_index, dad);
            writeRollAt(dad_index, bro);
            swapped= true;
        }
        file.flush();
        if(dad_index > 0 && swapped) {
            heapify_up(dad_index);
        }
    }

    int findMin() {
        if(readLength()>0) {
            return readRollFrom(0);
        } else {
            return INF;
        }
    }

    void deleteMin() {
        int length= readLength();
        if(length == 1) {
            writeRollAt(0, 0);
            writeLength(0);
            writeMaxRoll(0);
            // cout << "\nlen= 1";
            return ;
        } else if(length == 0) {
            return ;
        }

        int lastRoll= readRollFrom(length-1);
        writeRollAt(0, lastRoll);
        writeRollAt(length-1, 0);
        writeLength(length-1);

        heapify_dn(0, length-1);
    }

    void heapify_dn(int index, int length) {
        int child1_index, child2_index;
        if(index>= length) {
            return ;
        }
        child1_index= index*2+1;
        child2_index= child1_index+1;
        int child1_roll= child1_index<length? readRollFrom(child1_index): INF;
        int child2_roll= child2_index<length? readRollFrom(child2_index): INF;
        int index_roll= readRollFrom(index);
        int swapped_ind= 0;
        if(child1_roll < index_roll && child1_roll < child2_roll) {
            writeRollAt(child1_index, index_roll);
            writeRollAt(index, child1_roll);
            swapped_ind= child1_index;
        } else if(child2_roll< index_roll && child2_roll< child1_roll) {
            writeRollAt(child2_index, index_roll);
            writeRollAt(index, child2_roll);
            swapped_ind= child2_index;
        }
        if(swapped_ind) {
            heapify_dn(swapped_ind, length);
        }
    }

};


class DataBase {
    
private:
    fstream file;
    int node_len= 0, header_size= 0;
public:
    DataBase(string DB_Name) {
        bool new_db= false;
        file.open(DB_Name);
        // sometime, the file was not creating, so Quick FIX
        if(!file) {
            file.open(DB_Name, ios::out);
            file.close();
            file.open(DB_Name, ios::out | ios::in);
            new_db= true;
            cout << "\n\tCREATED NEW DB...\n\n";
        } else {
        	cout << "\n\nopened existing DB...\n\n";
        }
        if(!file) {
            cout << "\n\tError opening file- " << DB_Name << "\n";
            exit(-1);
        }
        node_len= 2+ 2*ROLL_LEN + 2 + 2*BST_MAX_LEN + 2 + HEAP_FILE_NAME_LEN + 1;
        header_size= HEAP_FILE_NAME_LEN+1;
        if(new_db) {
            init();
        }
    }
    ~DataBase() {
        if(file) {
            file.flush();
            file.close();
        }
    }

    void setRoll(int roll_no, int seek_pos) {
        char c[ROLL_LEN+1];
        c[ROLL_LEN]= ' ';
        num2arr(c, roll_no, ROLL_LEN);
        file.seekg(seek_pos);
        file.write(c, ROLL_LEN+1);
        file.flush();
    }

    int readData(int seek_pos, int length) {
        int res= 0;
        char c;
        for(int i=0; i<length; i++) {
            res*= 10;
            file.seekg(seek_pos+i);
            if(file.eof()) {
                break;
            }
            file.get(c);
            res += c-48;
        }
        return res;
    }

    void setChild(int child_location, int seek_pos) {
        char arr[BST_MAX_LEN+1];
        num2arr(arr, child_location, BST_MAX_LEN);
        arr[BST_MAX_LEN]= ' ';
        file.seekg(seek_pos);
        file.write(arr, BST_MAX_LEN+1);
        file.flush();
    }

    void setLeftChild(int child_location, int node_id) {
        setChild(child_location, header_size+ 2+ ROLL_LEN*2 + 2 + node_id* node_len);
    }

    int getLeftChild(int node_id) {
        return readData(header_size+ 2+ ROLL_LEN*2 + 2 + node_id* node_len, BST_MAX_LEN);
    }

    void setRightChild(int child_location, int node_id) {
        setChild(child_location,header_size+  2+ ROLL_LEN*2 + 2+ BST_MAX_LEN+ 1 + node_id* node_len);
    }

    int getRightChild(int node_id) {
        return readData(header_size+ 2+ ROLL_LEN*2 + 2+ BST_MAX_LEN+ 1 + node_id* node_len, BST_MAX_LEN);
    }

    void setMinKey(int roll_no, int node_id) {
        setRoll(roll_no,header_size+  2 + node_id* node_len);
    }

    int getMinKey(int node_id) {
        return readData(header_size+ 2 + node_id* node_len, ROLL_LEN);
    }

    void setMaxKey(int roll_no, int node_id) {
        setRoll(roll_no,header_size+ 2+ROLL_LEN +1 + node_id* node_len);
    }

    int getMaxKey(int node_id) {
        return readData(header_size+ 2+ROLL_LEN +1 + node_id* node_len, ROLL_LEN);
    }

    void setNodeId( int heapName, int node_id) {
        char arr[HEAP_FILE_NAME_LEN+1];
        num2arr(arr, heapName, HEAP_FILE_NAME_LEN);
        arr[HEAP_FILE_NAME_LEN]= '\n';
        file.seekg(header_size+ 2+ ROLL_LEN*2 + 2+ 2*BST_MAX_LEN+ 2 + node_id* node_len);
        file.write(arr, HEAP_FILE_NAME_LEN+1);
        file.flush();
    }

    int getNodeId(int node_id) {
        return readData(header_size+ 2+ ROLL_LEN*2 + 2+ 2*BST_MAX_LEN+ 2 + node_id* node_len, HEAP_FILE_NAME_LEN);
    }

    void getNodeFileName(char *arr, int node_id) {
        char c;
        for(int i=0; i<HEAP_FILE_NAME_LEN; i++) {
            file.seekg(header_size+ 2+ ROLL_LEN*2 + 2+ 2*BST_MAX_LEN+ 2 + node_id* node_len + i);
            file.get(c);
            arr[i]=c;
        }
        arr[HEAP_FILE_NAME_LEN]= '.';
        arr[HEAP_FILE_NAME_LEN+1]= 'd';
        arr[HEAP_FILE_NAME_LEN+2]= 'a';
        arr[HEAP_FILE_NAME_LEN+3]= 't';
        arr[HEAP_FILE_NAME_LEN+4]= '\0';
    }

    void changeType(char type, int node_id) { // type can be (c)hild or (i)nternal
        char str[3]= "  ";
        str[0]= type;
        file.seekg(header_size+ node_id* node_len);
        file.write(str, 2);
        file.flush();
    }

    char getType(int node_id) {
        file.seekg(header_size+ node_id* node_len);
        char c;
        file.get(c);
        return c;
    }

    int getNoOfNode() {
        return readData(0, HEAP_FILE_NAME_LEN);
    }

    int setNoOfNode(int node) {
        char tot_node[HEAP_FILE_NAME_LEN+1];
        num2arr(tot_node, node, HEAP_FILE_NAME_LEN);
        tot_node[HEAP_FILE_NAME_LEN]= '\n';
        file.seekg(0);
        file.write(tot_node, HEAP_FILE_NAME_LEN+1);
        file.flush();
    }

    void createNode(int node_id) {
        changeType(CHILD_NODE, node_id);
        setMinKey(0, node_id);
        setMaxKey(0, node_id);
        setLeftChild(0, node_id);
        setRightChild(0, node_id);
        setNodeId(0, node_id);
    }

    /***************************
     * Returns node ID
    ***************************/
    int createNode() {
        int node_id= getNoOfNode();
        
        changeType(CHILD_NODE, node_id);
        setMinKey(0, node_id);
        setMaxKey(0, node_id);
        setLeftChild(0, node_id);
        setRightChild(0, node_id);
        setNodeId(node_id, node_id);
        setNoOfNode(node_id+1);
        return node_id;
    }

    void init() {

        setNoOfNode(1);
        changeType(CHILD_NODE, 0);
        setMinKey(0, 0);
        setMaxKey(INF-1, 0);
        setLeftChild(0, 0);
        setRightChild(0, 0);
        setNodeId(0, 0);

        char fileName[HEAP_FILE_NAME_LEN+5];
        getNodeFileName(fileName, 0);

        Heaper* h= new Heaper(fileName);
        h->init();
        delete(h);
    }

    bool isValid(int node) {
        return getNoOfNode()>node;
    }

    // Returns node_id of a node where key can be inserted...
    int findNode(int key, int currentNode, bool inserting) {

        if(getType(currentNode) == CHILD_NODE) {
            return currentNode;
        }
        int leftChild= getLeftChild(currentNode);
        int rightChild= getRightChild(currentNode);
        if(inserting) {
            if(getMinKey(currentNode)> key) {
                setMinKey(key, currentNode);
            }
            if(getMaxKey(currentNode)< key) {
                setMaxKey(key, currentNode);
            }
        }
        if(getMinKey(rightChild)> key) {
            return findNode(key, leftChild, inserting);
        } else {
            return findNode(key, rightChild, inserting);
        }

    }

    bool insert(int key) {
        if(search(key)) {
            return false;
        }
        int currentNode= findNode(key, 0, true);
        char fileName[HEAP_FILE_NAME_LEN+5];
        getNodeFileName(fileName, currentNode);

        Heaper* h= new Heaper(fileName);
        if(currentNode == 0 && h->readLength() == 1) {
        	setMinKey(key, currentNode);
        	setMaxKey(key, currentNode);
        }
        if(h->readLength() < MAX_ROLL_IN_FILE) {
            h->insert(key);
            int min= getMinKey(currentNode);
            int max= getMaxKey(currentNode);
            if(key<min) {
                setMinKey(key, currentNode);
            }
            if(key>max) {
                setMaxKey(key, currentNode);
            }
            delete(h);
        } else {
            int newNode1= createNode();
            int newNode2= createNode();

            setLeftChild(newNode1, currentNode);
            setRightChild(newNode2, currentNode);

            char newName[HEAP_FILE_NAME_LEN+5];
            getNodeFileName(newName, newNode1);
            Heaper* h1= new Heaper(newName);
            h1->init();
            getNodeFileName(newName, newNode2);
            Heaper* h2= new Heaper(newName);
            h2->init();
            int min=INF-1, max= 0;
            for(int i=0; i<MAX_ROLL_IN_FILE/2; i++) {
                int tmpKey= h->findMin();
                h1->insert(tmpKey);
                if(tmpKey<min) {
                    min= tmpKey;
                }
                if(tmpKey > max) {
                    max= tmpKey;
                }
                h->deleteMin();
            }
            setMinKey(min, newNode1);
            setMaxKey(max, newNode1);
            delete(h1);

            min= INF-1; max= 0;
            for(int i=MAX_ROLL_IN_FILE/2; i<MAX_ROLL_IN_FILE; i++) {
                int tmpKey= h->findMin();
                h2->insert(tmpKey);
                if(tmpKey<min) {
                    min= tmpKey;
                }
                if(tmpKey > max) {
                    max= tmpKey;
                }
                h->deleteMin();
            }
            setMinKey(min, newNode2);
            setMaxKey(max, newNode2);
            delete(h2);
            
            delete(h);
            remove(fileName);
            changeType(INTERNAL_NODE, currentNode);
            insert(key);
        }
        return true;
    }


    bool search(int key) {
        int currentNode= findNode(key, 0, false);
        char heaperName[HEAP_FILE_NAME_LEN+5];
        getNodeFileName(heaperName, currentNode);
        Heaper* h= new Heaper(heaperName);
        if(h->search(key)) {
            delete(h);
            return true;
        }
        delete(h);
        return false;
    }

    void minAndMaxOfChilds(int node_id) {
        if(getType(node_id) == CHILD_NODE) {
            cout << "\n Node id- "<< node_id <<" min- " << getMinKey(node_id) << " max- " << getMaxKey(node_id);
            return ;
        }
        minAndMaxOfChilds(getLeftChild(node_id));
        minAndMaxOfChilds(getRightChild(node_id));
    }

    void minAndMaxOfChilds() {
        minAndMaxOfChilds(0);
    }

    void minAndMaxOfChilds2(int node_id) {
        if(getType(node_id) == CHILD_NODE) {
            char heaperName[HEAP_FILE_NAME_LEN+5];
            getNodeFileName(heaperName, node_id);
            Heaper* h= new Heaper(heaperName);
            cout << "\n Node id- "<< node_id <<" min- " ;
            cout << h->findMin();
            cout << " max- ";
            cout << h->readMaxRoll();
            delete(h);

            return ;
        }
        minAndMaxOfChilds(getLeftChild(node_id));
        minAndMaxOfChilds(getRightChild(node_id));
    }
    void minAndMaxOfChilds2() {
        minAndMaxOfChilds2(0);
    }

    void minAtAllNodes(int node_id) {
        cout << "\nNode id- " << node_id << " min- " << getMinKey(node_id);
        if(getType(node_id) == INTERNAL_NODE) {
            minAtAllNodes(getLeftChild(node_id));
            minAtAllNodes(getRightChild(node_id));
        }
    }

    void minAtAllNodes() {
        minAtAllNodes(0);
    }

    void maxAtAllNodes(int node_id) {
        cout << "\nNode id- " << node_id << " max- " << getMaxKey(node_id);
        if(getType(node_id) == INTERNAL_NODE) {
            maxAtAllNodes(getLeftChild(node_id));
            maxAtAllNodes(getRightChild(node_id));
        }
    }

    void maxAtAllNodes() {
        maxAtAllNodes(0);
    }

    void noOfNodes() {
        cout << "\n Total Nodes- " << getNoOfNode() ;
    }

    int treeHeight(int node_id) {
        if(getType(node_id)== CHILD_NODE) {
            return 1;
        }
        return max( treeHeight(getLeftChild(node_id)), treeHeight(getRightChild(node_id)));
    }

    void treeHeight() {
        cout << "\nHeight of tree- " << treeHeight(0);
    }

    int noOfLeaves(int node_id) {
        if(getType(node_id)== CHILD_NODE) {
            return 1;
        }
        return noOfLeaves(getLeftChild(node_id)) + noOfLeaves(getRightChild(node_id));
    }
    void noOfLeaves() {
        cout << "\nTotal no of leaves- " << noOfLeaves(0) ;
    }

    void printTree(int node_id, int no_of_tabs) {

        cout << "\n";
        for(int i=0; i<no_of_tabs; i++) {
            cout << "   ";
        }

        cout <<"+---Range = [ "<<getMinKey(node_id)<<" , "<<getMaxKey(node_id)<<" ], ";

        if(getType(node_id) == CHILD_NODE) {
            char heaperName[HEAP_FILE_NAME_LEN+5];
            getNodeFileName(heaperName, node_id);
            cout << "File- " << heaperName;
        } else {
            cout << "File- none" ;
            printTree(getLeftChild(node_id), no_of_tabs+1);
            printTree(getRightChild(node_id), no_of_tabs+1);
        }
    }

    void printTree() {
        cout <<"+++ BST ";
        printTree(0, 1);
    }

};




int main() {

    string DBName;

    cout << "Enter Database Name -";
    cin >> DBName;
    DataBase DB(DBName);

    int option= 1;
    int key;
    while(option > 0 && option<10) {
        showMenu();
        cin >> option;
        switch(option) {
        case 1:
            cout<< "Enter no of nodes to be inserted- ";
            int no_of_nodes;
            cin >> no_of_nodes ;
            for(int i=0; i<no_of_nodes; i++) {
                cout << "\nEnter "<<(i+1) <<"th key- ";
                cin >> key;
                if(DB.insert(key)) {
                    cout << "\tInserted "<<key<<" Successfully...";
                } else {
                    cout<< "\t Key- "<<key<<" Already Exists...";
                }
            }
            cout<< "\ndone Inserting\n\n";
            break;

        case 2:
            cout<< "Enter Valuee to be searched- ";
            cin >> key;
            if(DB.search(key)) {
                cout<< "\n\t The Key "<<key<< " Exists...\n\n";
            } else {
                cout<< "\n\t The Key "<<key<< " Does not Exists... :(\n\n";
            }
            break;

        case 3:
            DB.minAndMaxOfChilds();
            cout<<"\n\n";
            break;
        case 4:
            DB.minAtAllNodes();
            cout<<"\n\n";
            break;
        case 5:
            DB.maxAtAllNodes();
            cout<<"\n\n";
            break;
        case 6:
            DB.treeHeight();
            cout<<"\n\n";
            break;
        case 7:
            DB.noOfNodes();
            cout<<"\n\n";
            break;
        case 8:
            DB.noOfLeaves();
            cout<<"\n\n";
            break;
        case 9:
            DB.printTree();
            cout<<"\n\n";
            break;
        default:
            cout<< "\n\tWrong option, Exiting...";
        }
    }

}

void showMenu() {
    cout << "\n\n Enter Appropriate option or anything else to exit...\n";
    cout << "\t 1) Insert into DB\n";
    cout << "\t 2) Search From DB\n";
    cout << "\t 3) Get Min and Max of all leaves\n";
    cout << "\t 4) Get Min of all nodes\n";
    cout << "\t 5) Get Max of all nodes\n";
    cout << "\t 6) Get Height of tree\n";
    cout << "\t 7) Get no of nodes\n";
    cout << "\t 8) Get no of leaves\n";
    cout << "\t 9) print the tree\n";
    cout << "choose an option- ";
}