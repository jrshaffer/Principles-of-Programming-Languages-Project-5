#include "Helper2.h"
#include "node.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <cstddef>

using namespace std;

//string check = "";
//node *condCheck = new node;

int nodeLength(node *n) {
	int length = 0;
	//while (n != NULL) {
	while(n->value != "NIL") {
		if (n->left != NULL) {
			length++;
		}
		n = n->right;
	}
	return length;
}

node *Atom(node *n) {	
	node *temp = n;
	if (n->typeCheck == "Nat" || n->typeCheck == "Bool") {
		temp->typeCheck = "Bool";
		return temp;
	} else if(n->typeCheck == "List(Nat)") {
		temp->typeCheck = "Bool";
		return temp;
	} else { 
		cout << "TYPE CHECK ERROR: Undefined Value for Atom Function" << endl;
		exit(10);
	}
	return temp;
}

node *isNull1(node *n) {
	node *temp = n;
	if (n->typeCheck == "List(Nat)" || n->value == "NIL") {
		temp->typeCheck = "Bool";
		return temp;
	} else {
		cout << "TYPE CHECK ERROR: Undefined Value for NULL Function. Expected List of Numbers" << endl;
		exit(10);
	}
	return temp;
}

node *isInteger(node *n) {
	node *temp = n;
	if (n->typeCheck == "Nat" || n->typeCheck == "Bool" || n->typeCheck == "List(Nat)") {
		temp->typeCheck = "Bool";
		return temp;
	} else {
		cout << "TYPE CHECK ERROR: Undefined Value for INT Function" << endl;
		exit(10);
	}
	return temp;
}

node *car1(node *n) {
	node *temp = n;
	if (n->typeCheck == "List(Nat)") {
		temp->typeCheck = "Nat";
		return temp;
	} else {
		cout << "TYPE CHECK ERROR: Undefined Value for CAR Function" << endl;
		exit(10);
	}
	return temp;
}

node *cdr1(node *n) {
	node *temp = n;
	if (n->typeCheck == "List(Nat)") {
		temp->typeCheck = "List(Nat)";
		//temp->list = false;
		return temp;
	} else {
		cout << "TYPE CHECK ERROR: Undefined Value for CDR Function" << endl;
		exit(10);
	}
	return temp;
}

bool cons1(node *left, node *right) {
	if (left->typeCheck != "Nat") {
		cout << "TYPE CHECK ERROR: s1 expression is not a number" << endl;
		exit(10);
	} else if (right->typeCheck != "List(Nat)") {
		cout << "TYPE CHECK ERROR: s2 expression is not a list of numbers" << endl;
		exit(10);
	} else {
		return true;
	}
	return false;
}

node *cons(node *left, node *right) {
	node *root = new node;
	if (left == NULL) {
		root = right;
	} else if (right == NULL) {
		root = left;
	} else {
		root->left = left;
		root->right = right;
	}
	root->list = true;
	return root;
}

bool PLUS(node *left, node *right) {
	if (left->typeCheck != "Nat") {
		cout << "TYPE CHECK ERROR: s1 expression is not a number" << endl;
		exit(10);
	} else if (right->typeCheck != "Nat") {
		cout << "TYPE CHECK ERROR: s2 expression is not a number" << endl;
		exit(10);
	} else {
		return true;
	}
	return false;
}


bool eq(node *left, node *right) {
	if (left->typeCheck != "Nat") {
		cout << "TYPE CHECK ERROR: s1 expression is not a number" << endl;
		exit(10);
	} else if (right->typeCheck != "Nat") {
		cout << "TYPE CHECK ERROR: s2 expression is not a number";
		exit(10);
	} else {
		return true;
	}
	return false;
}

bool LESS(node *left, node *right) {
	if (left->typeCheck != "Nat") {
		cout << "TYPE CHECK ERROR: s1 expression is not a number" << endl;
		exit(10);
	} else if (right->typeCheck != "Nat") {
		cout << "TYPE CHECK ERROR: s2 expression is not a number" << endl;
		exit(10);
	} else {
		return true;
	}
	return false;
}


node *eval(node *root) {
	//cout << root->value << endl;
	node *temp = NULL;
	//cout << root->value << endl;
	if(root->type == "Atom" && root != NULL) {
		if (root->value == "T" || root->value == "F") {
			root->typeCheck = "Bool";
		} else if(root->atom == "Numeric") {
			root->typeCheck = "Nat";
		} else if(root->value == "NIL") {
			root->typeCheck = "List(Nat)";
		} else {
			cout << "TYPE CHECK ERROR: Undefined Atom for Eval Function" << endl;
			exit(10);
		}
		//root->list = false;
		temp = root;
	} else {
		if (car(root)->value == "COND") {
			try { 
				
				//temp = cond(cdr(root));	
				condType(cdr(root));
				temp = cons(car(root), cdr(root));
				//temp->list = false;
				temp->typeCheck = cdr(root)->check;
			} catch(exception& e) {
				cout << "TYPE CHECK ERROR: unkown error in COND function" << endl;
				exit(10);
			}
			
			
		} else {
			temp = apply(car(root), evalList(cdr(root)));
			temp->list = false;
		}
	}
	//temp = root;
	return temp;
}

void checkCond(node *root, string check) {
	//cout << check << endl;	
	if (root->value == "NIL") {
		return;
	}
	//node *temp = eval(car(car(root)));
	//temp = eval(car(cdr(car(root))));
	int length = nodeLength(car(root));
	//cout << length << endl;
	if (length != 2) {
		cout << "TYPE CHECK ERROR: Expression in COND Function does not have length of 2" << endl;
		exit(10);
	} else if (eval(car(car(root)))->typeCheck != "Bool") {
		cout << "TYPE CHECK ERROR: Undefined Bool Expression in COND Function" << endl;
		exit(10);
	} else if (eval(car(cdr(car(root))))->typeCheck != check) {
		cout << "TYPE CHECK ERROR: Undefined Type mismatch in COND Function" << endl;
		exit(10);
	} else { 
		checkCond(cdr(root), check);
	}
}

void condType(node *root) {
	node *temp = eval(car(car(root)));
	temp = eval(car(cdr(car(root))));
	if (!isNull(root)) {
		root->check = eval(car(cdr(car(root))))->typeCheck;
	}
	checkCond(root, root->check);
}

node *cond(node *root) {
	//cout << car(car(root))->value << endl;
	if(isNull(root)) {
		cout << "TYPE CHECK ERROR: NULL found in COND function" << endl;
		exit(10);
	} else if(eval(car(car(root)))->value != "NIL") {
		return eval(car(cdr(car(root))));
	} else {
		return cond(cdr(root));
	}
}

node *evalList(node *root) {
	if(isNull(root)) {
		node *temp = new node;
		temp->type = "Atom";
		temp->value = "NIL";
		temp->typeCheck = "List(Nat)";
		return temp;
	} else {
		return cons(eval(car(root)), evalList(cdr(root)));
	}
}

bool command(node *root) {
	string functions[14] = {"ATOM", "INT", "NULL", "CAR", "CDR", "CONS", "PLUS", "LESS", "EQ", "COND"};
	int i;
	for(i = 0; i < 14; i++) {
		if (functions[i] == root->value) {
			return true;
		} 
	}
	return false;
}

node *apply(node *function, node *parameters) {
	int length;
	length = nodeLength(parameters);
	node *temp = new node;
	if (command(function) == false) {
		cout << "TYPE CHECK ERROR: Function Undefined; First Element of s is not ATOM, INT, NULL, CAR, CDR, CONS, PLUS, LESS, EQ, or COND" << endl;
		exit(10);
	}
	if (function->value == "ATOM") {
		if (length != 1) {
			cout << "TYPE CHECK ERROR: more than 1 parameter found for ATOM function; Only 1 parameter expected" << endl;
			exit(10);
		}
		temp = Atom(car(parameters));
		//temp->list = true;
		temp = cons(function, temp);
		temp->typeCheck = "Bool";
		//temp->list = false;
	} else if(function->value == "NULL") {
		if (length != 1) {
			cout << "TYPE CHECK ERROR: more than 1 parameter found for NULL function; Only 1 parameter expected" << endl;
			exit(10);
		}
		//temp->list = true;
		temp = isNull1(car(parameters));
		temp = cons(function, temp);
		//temp->list = false;
		temp->typeCheck = "Bool";
	} else if(function->value == "INT") {
		if (length != 1) {
			cout << "TYPE CHECK ERROR: more than 1 parameter found for INT function; Only 1 parameter expected" << endl;
			exit(10);
		}
		temp = isInteger(car(parameters));
		//temp->list = true;
		temp = cons(function, temp);
		temp->typeCheck = "Bool";
		//temp->list = false;
	} else if(function->value == "PLUS") {
		if (length != 2) {
			cout << "TYPE CHECK ERROR: Only 2 parameters expected for PLUS function" << endl;
			exit(10);
		}
		if (PLUS(car(parameters), car(cdr(parameters)))) {
			temp = parameters;
			temp->typeCheck = "Nat";
		}
		//temp->list = true;
		temp = cons(function, temp);
		//temp->list = false;
		temp->typeCheck = "Nat";
	} else if(function->value == "EQ") {
		if (length != 2) {
			cout << "TYPE CHECK ERROR: Only 2 parameters expected for EQ function" << endl;
			exit(10);
		}
		if (eq(car(parameters), car(cdr(parameters)))) {
			temp = parameters;
			temp->typeCheck = "Bool";
		}
		//temp->list = true;
		temp = cons(function, temp);
		//temp->list = false;
		temp->typeCheck = "Bool";
	} else if(function->value == "LESS") {
		if (length != 2) {
			cout << "TYPE CHECK ERROR: Only 2 parameters expected for LESS function" << endl;
			exit(10);
		}
		if (LESS(car(parameters), car(cdr(parameters)))) {
			temp = parameters;
			temp->typeCheck = "Bool";
		}
		//temp->list = false;
		temp = cons(function, temp);
		//temp->list = true;
		temp->typeCheck = "Bool";
	} else if(function->value == "CAR") {
		if (length != 1) {
			cout << "TYPE CHECK ERROR: Only 1 parameter expected for CAR function" << endl;
			exit(10);
		}
		temp = car1(car(parameters));
		//temp->list = true;
		temp = cons(function, temp);
		//temp->list = false;
		temp->typeCheck = "Nat";
	} else if (function->value == "CDR") {
		if (length != 1) {
			cout << "TYPE CHECK ERROR: Only 1 parameter expected for CDR function" << endl;
			exit(10);
		}
		temp = cdr1(car(parameters));
		//temp->list = false;
		temp = cons(function, temp);
		//temp->list = false;
		temp->typeCheck = "List(Nat)";
	} else if(function->value == "CONS") {
		if (length != 2) {
			cout << "TYPE CHECK ERROR: Only 2 parameters expected for CONS function" << endl;
			exit(10);
		}
		if (cons1(car(parameters), car(cdr(parameters)))) {
			temp = parameters;
			temp->typeCheck = "List(Nat)";
		}
		//temp->list = true;
		temp = cons(function, temp);
		//temp->list = true;		
		temp->typeCheck = "List(Nat)";
	} else {
		//cout << "Function " << function->value;
		return function;
	}
	//cout << temp->left->value << endl;
	//temp->list = false;
	return temp;
}


bool isNull(node *n) {
	if (n->type == "Atom" && n->value == "NIL") {
		return true;
	} 
	return false;
}


node *car(node *n) {
	node *root = n;
	if (n != NULL) {
		if (n->type == "Atom") {
			cout << "TYPE CHECK ERROR: CAR function cannot be performed on atom" << endl;
			exit(10);
		} else {
			if (n->left != NULL) {
				root = n->left;
			} else {
				cout << "TYPE CHECK ERROR: unexpected error in CAR function" << endl;
				exit(10);
			}
		}
	} else {
		cout << "TYPE CHECK ERROR: undefined arguments in CAR function" << endl;
		exit(10);
	}
	return root;
}

node *cdr(node *n) {
	node *root = n;
	if (!isNull(n)) {
		if (n->type == "Atom") {
			cout << "TYPE CHECK ERROR: CDR function cannot be performed on atom" << endl;
			exit(10);
		} else {
			if (n->right != NULL) {
				root = n->right;
			} else {
				cout << "TYPE CHECK ERROR: unexpected error in CDR function" << endl;
				exit(10);
			}
		}
	} 
	return root;
}
