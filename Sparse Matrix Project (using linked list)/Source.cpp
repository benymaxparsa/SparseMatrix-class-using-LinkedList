#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

/*
		
		Data Structure's second project ( sparse matrix class made by using linked list )
					this code was writen by Parsa KamaliPour
								Sophmore(2nd) year Computer Engineering student at VRU (Vali-E-Asr University of Rafsanjan, Iran)
												solo member team
														contact:  telegram = @benymaxparsa  instagram = @benymax.parsa.public 
																			email = parsakamalipour@gmail.com

*/


struct SMNode {                // data for sparse matrix node if was chosen by uninon
	int row, col, val;
};

class LLSMNode                // sparse matrix node (linked list)
{
public:
	bool tag;                // if it's true : head node is chosen by union  / false : it's data node
	LLSMNode* down;          // its in coloumn head that down moves in it's rows
	LLSMNode* right;         // it's in row head that right moves in coloumns
	union                    // gives two option
	{
		LLSMNode* next;            // if it's head node , next connects to next head
		SMNode data;               // if it's data node , gives you row,coloumn,value
	} u;                     // union name is u

	
	LLSMNode(int maxCR);                   // data node constructor for making first head which is null just for addresing other main heads
	LLSMNode(int maxCR, bool flag);        // head node constructor for making max(row,col) numbers of normal head
	LLSMNode(int r, int c, int v);         // normal data node constructor
	
private:
};

LLSMNode::LLSMNode(int maxCR)               // null head for addresing others
{
	tag = NULL;                             // null because it isn't normal head
	down = NULL;                            // it's a null head
	right = new LLSMNode(maxCR, true);      // this null head right points to first normal head
	u.data.col = NULL;                   
	u.data.row = NULL;                      // this head is data node but it's null
	u.data.val = NULL;
}

LLSMNode::LLSMNode(int maxCR, bool flag)    // head nodes constructor
{
	if (maxCR <= 1) {                       // if matrix size is 1 make one head
		tag = flag;
		down = NULL;
		right = NULL;
		u.next = NULL;
	}
	else {                                  // if matrix size is bigger than 1 make n number of heads pointing to each other with "next"
		tag = flag;
		down = NULL;
		right = NULL;
		u.next = new LLSMNode(maxCR - 1, flag);
	}
}

LLSMNode::LLSMNode(int r, int c, int v)        // normal data node constructor
{
	tag = false;
	down = NULL;
	right = NULL;
	u.data.col = c;
	u.data.row = r;
	u.data.val = v;
}


class LLSparseMatrix      // sparse matrix class
{
public:
	LLSMNode* head;       // matrix's first head which is null type
	int Rows, Cols;       // original matrix sizes
	int max;              // maximum size between row and col

	LLSparseMatrix();                          // null matrix constructor
	LLSparseMatrix(int Rows, int Cols);        // sparse matrix constructor that has size of rows and cols and is going to make max number of heads plus a null head

	bool addTerm(int r, int c, int v);         // add a term to your sparse matrix
	void transpose();                          // transpose
	void add(LLSparseMatrix* other);           // if two matrixes has same size each node will be summed
	void subtract(LLSparseMatrix* other);      // if two matrixes has same size each node will be subtracted from each other
	void multi(LLSparseMatrix* other);         // multiply to matrices
	void copy(LLSparseMatrix* other);          // replace new matrix with old one
	void show();                               // show your sparse matrix
	void showMatrix();                         // show your sparse matrix's original matrix
	LLSMNode* search(int r, int c);            // finds node of Rth row and Cth col in your sparse matrix

private:
};

LLSparseMatrix::LLSparseMatrix()                              // null matrix constructor
{
	Rows = NULL;
	Cols = NULL;
	max = NULL;
	head = NULL;
}

LLSparseMatrix::LLSparseMatrix(int Rows, int Cols)          // sparse matrix constructor that has size of rows and cols        
{
	this->Rows = Rows;                                      // sets rows size
	this->Cols = Cols;                                      // sets cols size  
	if (Rows <= Cols)
		max = Cols;
	else                                                    // finds maximum size between rows and cols
		max = Rows;
	head = new LLSMNode(max);                               // makes our sparse matrix it's first head which is null type
}

bool LLSparseMatrix::addTerm(int r, int c, int v)           // add a term to your sparse matrix
{
	if (r >= Rows || c >= Cols)                             // if the row or coloumn entered is bigger than our array return false
		return false;

	LLSMNode* newNode = new LLSMNode(r, c, v);              // make a new temp node and set it's row, col and value to use it later

	LLSMNode* p = head->right;                              // make a pointer node pointing to the first real head(first row)
	int k = 0;
	while (k != r){                                         //as long as we haven't reached the entered row go on
		p = p->u.next;                                      // move next through heads (next row)
		k++;
	}                                                       // we have reached our wanted row (head)
	if (!p->right)                                          // if our wanted row doesn't have any coloumn
		p->right = newNode;                                 // add new col to it which is pointing to that temp data node
	else {                                                  // if our row have some col
		while (p->right && p->right->u.data.col < c)        // as long as there's new next col and it's col's index is smaller than our entered col, go on
			p = p->right;                                   // pointer node points to next col
		if (!p->right)                                      // if there isn't any next col left
			p->right = newNode;                             // make next col point to the temp data node
		else if (p->right->u.data.col == c) {               // if next col's index is equal to our entered col's index
			p->right->u.data.val = v;                       // set our new value to it
			return true;                                    // end the addTerm function with returning true
		}
		else                                                // if there's next col and it's index is bigger than our entered col index 
		{
			newNode->right = p->right;                      // put our new temp data node after our pointer it make our pointer node point to the new data node
			p->right = newNode;                             // then put pointer next col node after our new data node and make our data node point to it
		}
	}
	p = head->right;                                        // reset pointer node to first head(first coloumn)
	k = 0;
	while (k != c) {                                        //as long as we haven't reached the entered col go on
		p = p->u.next;                                      // move next through heads (next col)
		k++;
	}                                                       // we have reached our wanted col (head)
	if (!p->down) {                                         // if our wanted col doesn't have any row
		p->down = newNode;                                  // add new row to it which is pointing to that temp data node
		return true;                                        // end the addTerm function with returning true
	}
	else {                                                  // if our col have some rows
		while (p->down && p->down->u.data.row < r)          // as long as there's next row and it's index is smaller than entered row index go on
			p = p->down;                                    // go down throught row and update pointer
		if (!p->down) {                                     // if there's no down row left anymore  
			p->down = newNode;                              // point our col's down(row) to new temp data node 
			return true;   
		}
		else if (p->down->u.data.row == r) {                // if next col's down row index is equal to our entered row index
			p->down->u.data.val = v;                        // update the node's value to our newly entered value
			return true;
		}
		else                                                // if there's next row(down) for our col and it's index is bigger than our entered row index
		{
			newNode->down = p->down;                      // put our new temp data node under our pointer and make our pointer node point to the new data node
			p->down = newNode;                            // then put pointer next row node under our new data node and make our data node point to it
			return true;
		}
	}

}

void LLSparseMatrix::transpose()
{
	LLSMNode* p = head->right;
	while (p)
	{
		LLSMNode* temp = p->down;
		p->down = p->right;
		p->right = temp;

		LLSMNode* q = p->down;
		while (q)
		{

			int t = q->u.data.row;
			q->u.data.row = q->u.data.col;
			q->u.data.col = t;

			temp = q->down;
			q->down = q->right;
			q->right = temp;

			q = q->down;

		}
		p = p->u.next;
	}
}

void LLSparseMatrix::add(LLSparseMatrix* other)                                    // if two matrixes has same size each node will be summed
{
	LLSMNode* p;
	LLSMNode* q;
	if (Rows == other->Rows && Cols == other->Cols) {
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Cols; j++)
			{
				p = search(i, j);
				q = other->search(i, j);
				if (q) {
					if (!p)
						addTerm(q->u.data.row, q->u.data.col, q->u.data.val);
					else
						p->u.data.val = p->u.data.val + q->u.data.val;
				}
			}
		}
	}
}

void LLSparseMatrix::subtract(LLSparseMatrix* other)                               // if two matrixes has same size each node will be subtracted from each other
{
	LLSMNode* p;
	LLSMNode* q;
	if (Rows == other->Rows && Cols == other->Cols) {
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Cols; j++)
			{
				p = search(i, j);
				q = other->search(i, j);
				if (q) {
					if (!p) {
						q->u.data.val = q->u.data.val * -1;
						addTerm(q->u.data.row, q->u.data.col, (-1 * q->u.data.val));
					}
					else
						p->u.data.val = p->u.data.val - q->u.data.val;
				}
			}
		}
	}
}

void LLSparseMatrix::multi(LLSparseMatrix* other)                            // multiply to matrices
{
	if (Cols != other->Rows)
	{
		cout << "can't multiply" << endl;
		return;
	}
	else {
		LLSparseMatrix* node = new LLSparseMatrix(Rows, other->Cols);
		int** temp = new int*[Rows];
		for (int i = 0; i < Rows; i++)
		{
			temp[i] = new int[Cols];
			for (int j = 0; j < Cols; j++)
			{
				temp[i][j] = 0;
			}
		}
		for (int r = 0; r < Rows; r++)
		{
			for (int oc = 0; oc < other->Cols; oc++)
			{
				for (int c = 0; c < Cols; c++)
				{
					if (search(r, c) && search(c, oc))
						temp[r][oc] += (search(r, c)->u.data.val) * (other->search(c, oc)->u.data.val);
				}	
			}
		}
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < other->Cols; j++)
			{
				addTerm(i, j, temp[i][j]);
			}
		}
	}
}

void LLSparseMatrix::copy(LLSparseMatrix* other)                               // replace new matrix with old one
{
	LLSparseMatrix* node = new LLSparseMatrix(Rows, Cols);
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{	
			if (other->search(i, j))
				node->addTerm(i, j, other->search(i, j)->u.data.val);
		}
	}
	head = node->head;
}         

void LLSparseMatrix::show()                                       // show your sparse matrix
{
	LLSMNode* p = head->right;                                    // makes a pointer that points to first raed head
	while (p)                                                     // go until you walk through all heads and there's no head left
	{
		LLSMNode* q = p->right;                                   // makes a pointer that points to Nth head's (Nth row) right
		while (q) {                                               // go until you walk through all rights (coloumns) and there's no data node here left 
		
			cout << q->u.data.row << " " << q->u.data.col << " " << q->u.data.val << " / ";    // prints row of our sparse matrix
			q = q->right;                                                                      // move to next coloumn (right)
		}
		p = p->u.next;                                            // move to next row (head)
		cout << endl;                                             // print next row on a new line
	}
}

void LLSparseMatrix::showMatrix()                       // show your sparse matrix's original matrix
{
	LLSMNode* p;                                        // make a node pointer
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0 ; j < Cols; j++)
		{
			p = search(i, j);                           // search in matrix to find any node 
			if (!p) {
				cout << "0" << "   ";                   // if node was not found print 0 
			}
			else
				cout << p->u.data.val << "   ";         // if node were found print it's value in matrix
		}
		cout << endl;
	}
}

LLSMNode* LLSparseMatrix::search(int r, int c)        // finds node of Rth row and Cth col in your sparse matrix
{
	LLSMNode* p = head->right;                        // set node pointer to first read head
	for (int i = 0; i < r; i++) {
		p = p->u.next;                                // go next through heads until you reach the wanted row
	}
	LLSMNode* q = p->right;                           // set 2nd node pointer to our Nth head(row) right which is our first coloumn
	while (q) {                                       // as long as there's right dir , go on
		if (q->u.data.col == c)                       // if it has reach our wanted col  
			return q;                                 // return it's node
		q = q->right;                                 // if not, go through next coloumn dir (next right)
	}
	return NULL;                                      // if it hasn't find this wanted node return null
}

LLSparseMatrix* readFile() {
	string line;
	LLSparseMatrix* p = NULL;
	ifstream myfile("in.txt");
	if (myfile.is_open())
	{
		getline(myfile, line);
		int row = stoi(line);

		getline(myfile, line);
		int col = stoi(line);
		p = new LLSparseMatrix(row, col);
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				string temp;
				myfile >> temp;
				int num = stoi(temp);
				if (num)
				{
					p->addTerm(i, j, num);
				}
			}
			getline(myfile, line);
		}
		myfile.close();
	}
	return p;
}

LLSparseMatrix* file = NULL;
LLSparseMatrix* other = NULL;
LLSparseMatrix* otherM = NULL;
LLSparseMatrix* otherC = NULL;
LLSparseMatrix* otherS = NULL;

void app() {

	int toDo;

	cout <<endl<< "what do you want to do?" << endl;
	cout << "1. read matrix from file" << endl << "2. add term to sparse matrix" << endl;
	cout << "3. transpose" << endl << "4 .add the SM to another one" << endl << "5. multiply this SM to another one" << endl << "6. copy" << endl;
	cout << "7. show sparse matrix" << endl << "8. show SM converted to normal matrix" << endl;
	cout <<"9. subtract SM" <<endl << "10. restart app" << endl << "0. exit app" << endl << endl;
	
	cin >> toDo;

	switch (toDo)
	{
	case 0:
		return;
	case 10:
		system("cls");
		app();
		break;
	case 1:         // read file
		//readFile();
		file = readFile();
		if (file)
			cout << "file has been read sucsusfuly" << endl;
		else
			cout << "no file where found" << endl;
		app();
		break;
	case 2:
		// addterm
		int r, c, v;
		cin >> r >> c >> v;
		file->addTerm(r, c, v);
		cout << "term added" << endl;
		app();
		break;
	case 3:
		//transpose
		file->transpose();
		cout << "SM got transposed" << endl;
		app();
		break;
	case 4:
		// add
		cout << "change matrix txt file then press any key" << endl;
		getchar();
		other = readFile();
		if (other) {
			cout << "second file has been read sucsusfuly" << endl;
			file->add(other);
			cout << "two matrices has gotten add together" << endl;
		}
		else
			cout << "no second file where found" << endl;

		app();
		break;
	case 5:
		// multi
		cout << "change matrix txt file then press any key" << endl;
		getchar();
		otherM = readFile();
		if (otherM) {
			cout << "second file has been read sucsusfuly" << endl;
			file->multi(otherM);
			cout << "two matrices has gotten multied together" << endl;
		}
		else
			cout << "no second file where found" << endl;

		app();
		break;
	case 6:
		// copy matrix
		cout << "change matrix txt file then press any key" << endl;
		getchar();
		otherC = readFile();
		if (otherC) {
			cout << "second file has been read sucsusfuly" << endl;
			file->copy(otherC);
			cout << "matrix has gotten copied" << endl;
		}
		else
			cout << "no second file where found" << endl;
		app();
		break;
	case 7:
		//show sparse matrix
		file->show();
		app();
		break;
	case 8:
		// show normal matrix
		file->showMatrix();
		app();
		break;
	case 9:
		//subtract
		cout << "change matrix txt file then press any key" << endl;
		getchar();
		otherS = readFile();
		if (otherS) {
			cout << "second file has been read sucsusfuly" << endl;
			file->subtract(otherS);
			cout << "two matrices has gotten subtracted from each other" << endl;
		}
		else
			cout << "no second file where found" << endl;

		app();
		break;

	default:
		cout << "wrong input"<<endl;
		app();
		break;
	}
}

void theEnd() {
	cout << "Data Structure's second project ( sparse matrix class made by using linked list)" << endl << "   this code was writen by Parsa KamaliPour" << endl;
	cout << "      Sophmore(2nd) year Computer Engineering student at VRU(Vali - E - Asr University of Rafsanjan, Iran)" << endl;
	cout << "            solo member team" << endl;
	cout << "contact : telegram = @benymaxparsa  instagram = @benymax.parsa.public" << endl << "   email = parsakamalipour@gmail.com" << endl <<endl <<endl;
}

int main() {
	
	cout << "welcome to sparse matrix project app!     ----     made by Parsa KamaliPour" << endl;
	app();
	cout << endl;
	theEnd();
	return 0;
}
