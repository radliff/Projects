#include <iostream>
#include<string>
#include <vector>
#include <queue>
#include <fstream>
#include <regex>
using namespace std;


// every node contains a Student object w/ their name & ID
struct Student{
    string name;
    string ID;
    Student(){};
    Student(string name, string ID);
};

Student::Student(string name, string ID) {
    this->name = name;
    this->ID = ID;
}

// cited from the code given in class
class BST
{
private:
    struct TreeNode
    {
        Student val;
        TreeNode *left;
        TreeNode *right;
        TreeNode(string name, string ID) : val(name, ID), left(nullptr), right(nullptr) {}
    };
    TreeNode* root = nullptr;
    BST::TreeNode* helperInsert(TreeNode* helpRoot, const string& name, const string& ID, vector<TreeNode*> &insertedRoots);
    void helperInOrder(TreeNode* helpRoot, vector<TreeNode*> &returnVec);
    void helperPreOrder(TreeNode* helpRoot, vector<TreeNode*> &returnVec);
    void helperPostOrder(TreeNode* helpRoot, vector<TreeNode*> &returnVec);
    BST::TreeNode* helperSearchID(TreeNode* helpRoot, string searchID);
    void helperSearchName(TreeNode* helpRoot, const string& searchName, vector<string> &matchNames);
    void helperDelete(TreeNode* &helpRoot, const string& searchID, vector<TreeNode*> &deletedRoots);
    int helperLevelCount(TreeNode* helpRoot);
    void helperRemoveInOrder(TreeNode* &helpRoot, int &n, vector<TreeNode*> &deletedRoots);
    TreeNode* rotateLeft(TreeNode* &node);
    TreeNode* rotateRight(TreeNode* &node);
public:
    vector<BST::TreeNode*> inorder();
    vector<BST::TreeNode*> preorder();
    vector<BST::TreeNode*> postorder();
    vector<BST::TreeNode*> insert(string name, string ID);
    void searchID(string searchID);
    vector<string> searchName(string name);
    vector<BST::TreeNode*> removeID(string ID);
    void printLevelCount();
    vector<TreeNode *> removeInOrder(int n);
    // added this return vector for Catch2 test cases
    vector<BST::TreeNode*> returnVec;
};

// both level count & height finder
int BST::helperLevelCount(TreeNode* helpRoot) {
    // base case: we've gotten to the end of the tree
    if (helpRoot == nullptr){
        return 0;
    }
    // finding the height of the left subtree
    int maxLeft = helperLevelCount(helpRoot->left);

    // finding the height of the right subtree
    int maxRight = helperLevelCount(helpRoot->right);

    // return the max of either of these for each root, we should find the height
    // also add one for the current level
    return 1 + max(maxLeft, maxRight);
}

// cited from the given lecture slides (slide 12 on balanced trees)
BST::TreeNode* BST::rotateLeft(BST::TreeNode* &node) {
    // check for nullptr
    if (node == nullptr){
        return node;
    }
    TreeNode* grandchild = node->right->left;
    TreeNode* newParent = node->right;
    newParent->left = node;
    node->right = grandchild;
    return newParent;
}


BST::TreeNode* BST::rotateRight(BST::TreeNode *&node) {
    //  check for nullptr
    if (node == nullptr){
        return node;
    }
    // opposite of left rotation
    TreeNode* grandchild = node->left->right;
    TreeNode* newParent = node->left;
    newParent->right = node;
    node->left = grandchild;
    return newParent;
}

// helper function to insert node and check balance factor for AVL tree
BST::TreeNode* BST::helperInsert(BST::TreeNode *helpRoot, const string& name, const string& ID, vector<TreeNode*> &insertedNodes) {
    if (helpRoot == nullptr){
        insertedNodes.push_back(new TreeNode(name, ID));
        return new TreeNode(name,ID);
    }
    // Comparing the id thats being passed in to the id that is already in there
    // Is the ID less than the current node val?
    else if (ID.compare(helpRoot->val.ID) < 0){
        helpRoot->left = helperInsert(helpRoot->left, name, ID, insertedNodes);
    }
    // Is the ID greater than the current node val?
    else if (ID.compare(helpRoot->val.ID) > 0){
        helpRoot->right = helperInsert(helpRoot->right, name, ID, insertedNodes);
    }
    //  check the height of the deepest node's right and left subtrees
    int heightLeft = helperLevelCount(helpRoot->left);
    int heightRight = helperLevelCount(helpRoot->right);

  // right heavy
    if (heightLeft - heightRight < -1){
      // check if right subtree is left heavy
       if (helperLevelCount(helpRoot->right->left) - helperLevelCount(helpRoot->right->right) >= 1){
           // perform right left rotation
           helpRoot->right = rotateRight(helpRoot->right);
           helpRoot = rotateLeft(helpRoot);
       } else {
           // perform left rotation
           helpRoot = rotateLeft(helpRoot);
       }
    }
    // left heavy
    if (heightLeft - heightRight > 1){
        // check if left subtree is right heavy
        if (helperLevelCount(helpRoot->left->left) - helperLevelCount(helpRoot->left->right) <= -1){
            // perform left right rotation
            helpRoot->left = rotateLeft(helpRoot->left);
            helpRoot = rotateRight(helpRoot);
        }
        else {
            // perform right rotation
            helpRoot = rotateRight(helpRoot);
        }
    }
    return helpRoot;
}

// helper function to ensure inorder: left, node, right
void BST::helperInOrder(BST::TreeNode *helpRoot, vector<TreeNode*> &roots) {
    if (helpRoot == nullptr){
        cout << "";
    }
    else {
        helperInOrder(helpRoot->left, roots);
        roots.push_back(helpRoot);
        helperInOrder(helpRoot->right, roots);
    }
}

// helper function to ensure preorder: node, left, right
void BST::helperPreOrder(BST::TreeNode *helpRoot, vector<TreeNode*> &roots){
    if (helpRoot == nullptr){
        cout << "";
    }
    else {
        roots.push_back(helpRoot);
        helperPreOrder(helpRoot->left, roots);
        helperPreOrder(helpRoot->right, roots);
    }
}

// helper function to ensure postorder: left, right, node
void BST::helperPostOrder(BST::TreeNode *helpRoot, vector<TreeNode*> &roots) {
    if (helpRoot == nullptr) {
        cout << "";
    }
    else {
        helperPostOrder(helpRoot->left, roots);
        helperPostOrder(helpRoot->right, roots);
        roots.push_back(helpRoot);
    }
}

// finds name searching for IDS
BST::TreeNode* BST::helperSearchID(BST::TreeNode *helpRoot, string searchID) {
    if (helpRoot == nullptr || helpRoot->val.ID == searchID){
        return helpRoot;
    }
    // is the id we search for less than the current root?
    if (searchID.compare(helpRoot->val.ID) < 0){
        return helperSearchID(helpRoot->left, searchID);
    }
    // greater than current root
    return helperSearchID(helpRoot->right, searchID);
}

// searches for every ID associated w/ name & prints them
void BST::helperSearchName(BST::TreeNode *helpRoot, const string &searchName, vector<string> &matchNames) {
    if (helpRoot == nullptr){
        return;
    }
    // preorder traverse the entire tree
    if (helpRoot->val.name == searchName){
        cout << helpRoot->val.ID << "\n";
        // this vector is to check if the function has actually found any matching nodes
        matchNames.push_back(helpRoot->val.ID);
    }
    // search to the left and right nodes
    helperSearchName(helpRoot->left, searchName, matchNames);
    helperSearchName(helpRoot->right, searchName, matchNames);

}

// Olog(n)
void BST::helperDelete(BST::TreeNode *&helpRoot, const string& searchID, vector<TreeNode*> &deletedRoots) {
    // if it ever gets to this point, ID is not in the tree
    if (helpRoot == nullptr){
        cout << "unsuccessful";
        return;
    }
    // node to be deleted is less than current id
    if (searchID.compare(helpRoot->val.ID) < 0){
        helperDelete(helpRoot->left, searchID, deletedRoots);
    }
    // greater than current id
    else if (searchID.compare(helpRoot->val.ID) > 0){
        helperDelete(helpRoot->right, searchID, deletedRoots);
    }
    // we found the node to delete
    else {
        // case where removed root has no children
        if (helpRoot->left == nullptr && helpRoot->right == nullptr){
            deletedRoots.push_back(helpRoot);
            helpRoot = nullptr;
            delete helpRoot;
        }
        // case where removed root has one child
        else if (helpRoot->left == nullptr || helpRoot->right == nullptr){
            // if the left is not null, copy its value to the current node and delete the left
            if (helpRoot->left != nullptr){
                deletedRoots.push_back(helpRoot);
                helpRoot->val.ID = helpRoot->left->val.ID, helpRoot->val.name = helpRoot->left->val.name;
                helpRoot->left = nullptr;
                delete helpRoot->left;
            }
            // same process for the right
            if (helpRoot->right != nullptr){
                // vector to keep track of deleted root
                deletedRoots.push_back(helpRoot);
                helpRoot->val.ID = helpRoot->right->val.ID, helpRoot->val.name = helpRoot->right->val.name;
                helpRoot->right = nullptr;
                delete helpRoot->right;
            }
        }
        // case where removed root has two children
        else{
            // right child has no left; its the inorder successor
            if (helpRoot->right->left == nullptr){
                deletedRoots.push_back(helpRoot);
                helpRoot->val.ID = helpRoot->right->val.ID, helpRoot->val.name = helpRoot->right->val.name;
                helperDelete(helpRoot->right, helpRoot->right->val.ID,deletedRoots);
            // set right child leftmost to be inorder successor, then run the function again to delete the node
            } else {
                // init temp root to hold deleted value
                TreeNode* temp = helpRoot->right;
                deletedRoots.push_back(helpRoot);
                // looking for inorder successor - leftmost child
                while (temp->left != nullptr){
                    temp = temp->left;
                }
                // replacing helpRoot w/ inorder successor value
                helpRoot->val.ID = temp->val.ID, helpRoot->val.name = temp->val.name;
                // we now delete the inorder successor w/ the value
                helperDelete(helpRoot->right, temp->val.ID, deletedRoots);
            }
        }
    }
}

void BST::helperRemoveInOrder(BST::TreeNode* &helpRoot, int &n, vector<TreeNode*> &deletedRoots) {
    // base case
    if (helpRoot == nullptr || n < 0) {
        return;
    }

    // traverse the tree in order & only decrement when we reach the farthest node to the left
    helperRemoveInOrder(helpRoot->left, n, deletedRoots);
    // when n is 0 we are at the node to delete
    if (n == 0){
        helperDelete(helpRoot, helpRoot->val.ID, deletedRoots);
        // set n to negative 1 for a stopping condition
        n = -1;
        return;
    }
    n = n-1;
    // look at right subtree
    helperRemoveInOrder(helpRoot->right, n, deletedRoots);
}

// inserting node into tree
vector<BST::TreeNode*> BST::insert(string name, string ID){
    vector<TreeNode*> nodes;
    this->root = helperInsert(this->root, name, ID,nodes);
    return nodes;
}

// printing nodes inorder
vector<BST::TreeNode*> BST::inorder() {
    // testing purposes: make tree is being traversed correctly
    vector <TreeNode*> orderedRoots;
    helperInOrder(this->root, orderedRoots);
    for (size_t i = 0; i < orderedRoots.size(); i++){
        if (i == orderedRoots.size() - 1){
            cout << orderedRoots[i]->val.name;
        } else {
            cout << orderedRoots[i]->val.name << ", ";
        }
    }
    cout << endl;
    return orderedRoots;
}

// printing nodes preorder
vector<BST::TreeNode*> BST::preorder(){
    // testing purposes: make tree is being traversed correctly
    vector <TreeNode*> orderedRoots;
    helperPreOrder(this->root, orderedRoots);
    for (size_t i = 0; i < orderedRoots.size(); i++){
        if (i == orderedRoots.size() - 1){
            cout << orderedRoots[i]->val.name;
        } else {
            cout << orderedRoots[i]->val.name << ", ";
        }
    }
    cout << endl;
    return orderedRoots;
}

// printing nodes postorder
vector<BST::TreeNode*> BST::postorder() {
    // testing purposes: make tree is being traversed correctly
    vector <TreeNode*> orderedRoots;
    helperPostOrder(this->root, orderedRoots);
    for (size_t i = 0; i < orderedRoots.size(); i++){
        if (i == orderedRoots.size() - 1){
            cout << orderedRoots[i]->val.name;
        } else {
            cout << orderedRoots[i]->val.name << ", ";
        }
    }
    cout << endl;
    return orderedRoots;
}

// search for specific ID, print name associated w/ it
void BST::searchID(string searchID){
    // create a new TreeNode object to hold the value of searchID
    TreeNode* foundRoot = helperSearchID(this->root, searchID);
    if (helperSearchID(this->root, searchID) == nullptr){
        cout << "unsuccessful" << endl;
        return;
    }
    cout << foundRoot->val.name << endl;
}

// search for name, return a vector with all roots that have that name
vector<string> BST::searchName(string name) {
    // create vector to hold matched name, empty = unsuccessful search
    vector<string> IDRoots;
    helperSearchName(this->root, name, IDRoots);
    if (IDRoots.empty()){
        cout << "unsuccessful" << endl;
    }
    return IDRoots;
}

// removing a node based on ID
vector<BST::TreeNode*> BST::removeID(string ID) {
    vector <TreeNode*> deletedRoots;
    helperDelete(this->root, ID, deletedRoots);
    return deletedRoots;
}

void BST::printLevelCount() {
    cout << helperLevelCount(this->root);
}

// removes nodes in tree based on inorder traversal
vector<BST::TreeNode *> BST::removeInOrder(int n) {
    vector <TreeNode*> deletedRoots;
    helperRemoveInOrder(this->root, n, deletedRoots);
    if (deletedRoots.empty()){
        cout << "unsuccessful\n";
    } else {
        cout << "successful\n";
    }
    return deletedRoots;
}

// verify the name of commands passed in
bool verifyName(const string& name){
    // name only has alphabetic characters
    regex obj = regex("^[A-Za-z\\s]+$");
    return regex_search(name, obj);
}

// verify ID of command passed in
bool verifyID(const string& ID){
    // ID needs to be 8 characters
    if (ID.length() != 8){
        return false;
    }
    // ID is only numeric
    regex obj = regex("^[0-9]*$");
    return regex_search(ID, obj);
}

// this is a test function for catch2
// should return false for any unsuccessful commands
bool correctCommand(string command, BST tree){
    // read in command
    cin >> command;

    // user insert
    if (command == "insert"){
        string name;
        // reading until first quote
        getline(cin ,name, '"');

        // now we read the name until the last quote
        getline(cin, name, '"');

        // name validation
        if (!verifyName(name)){
            cout << "unsuccessful" << endl;
            return false;
        }
        string ID;
        Student s;
        getline(cin, ID, ' ');
        getline(cin, ID);

        // ID validation
        if (!verifyID(ID)){
            cout << "unsuccessful" << endl;
            return false;
        } else {
            s.name = name, s.ID = ID;
            tree.insert(name, ID);
            cout << "successful" << endl;
        }
    }
        // user printInorder
    else if (command == "printInorder"){
        tree.inorder();
    }
        // user printPreorder
    else if (command == "printPreorder"){
        tree.preorder();
    }
        // user printPostorder
    else if (command == "printPostorder"){
        tree.postorder();
    }
        // user remove
    else if (command == "remove"){
        string ID;
        string next;
        getline(cin, next, ' ');
        getline(cin, ID);
        if (!verifyID(ID)){
            cout << "unsuccessful" << endl;
            return false;
        } else {
            cout << "successful" << endl;
            tree.removeID(ID);
        }
    }
        // user printLevelCount
    else if (command == "printLevelCount"){
        tree.printLevelCount();
        cout << endl;
    }
        // user removeInorder
    else if (command == "removeInorder"){
        int n;
        string next;
        getline(cin, next, ' ');
        cin >> n;
        tree.removeInOrder(n);
    }
        // user searchID or search NAME
    else if (command == "search") {
        // skip the space after the command
        cin.ignore(1, ' ');
        // if next character is a quote, we know it's a name
        if (cin.peek() == '"') {
            // skip quote
            cin.ignore();

            // Read the name until the next quote
            string name;
            getline(cin, name, '"');

            // Attempt to search by name
            if (!verifyName(name)) {
                cout << "unsuccessful" << endl;
            } else {
                tree.searchName(name);
            }
        } else {
            // Read the ID otherwise
            string ID;
            getline(cin, ID);

            // Attempt to search by ID
            if (!verifyID(ID)) {
                cout << "unsuccessful" << endl;
            } else {
                tree.searchID(ID);
            }
        }
    }
        // wrong command
    else {
        cout << "unsuccessful." << endl;
        return false;
    }
    return true;
}

int main(){
    BST tree;
    // iterate for # of lines
    // read the entire line
    // find position of first space: from beginning of line to space is the first command
    // name: first quote to last quote
    // UFID: last space to the end
    int lines;
    cin >> lines;
    for (int i = 0; i < lines; i++){
        // read in command
        string command;
        cin >> command;

        // user insert
        if (command == "insert"){
            string name;
            // reading until first quote
            getline(cin ,name, '"');

            // now we read the name until the last quote
            getline(cin, name, '"');

            // name validation
            if (!verifyName(name)){
                cout << "unsuccessful" << endl;
            }
            string ID;
            Student s;
            getline(cin, ID, ' ');
            getline(cin, ID);

            // ID validation
            if (!verifyID(ID)){
                cout << "unsuccessful" << endl;
            } else {
                s.name = name, s.ID = ID;
                tree.insert(name, ID);
                cout << "successful" << endl;
            }
        }
        // user printInorder
        else if (command == "printInorder"){
            tree.inorder();
        }
        // user printPreorder
        else if (command == "printPreorder"){
            tree.preorder();
        }
        // user printPostorder
        else if (command == "printPostorder"){
            tree.postorder();
        }
        // user remove
        else if (command == "remove"){
            string ID;
            string next;
            getline(cin, next, ' ');
            getline(cin, ID);
            // check ID input
            if (!verifyID(ID)){
                cout << "unsuccessful" << endl;
            } else {
                tree.removeID(ID);
                cout << "successful" << endl;
            }
        }
        // user printLevelCount
        else if (command == "printLevelCount"){
            tree.printLevelCount();
            cout << endl;
        }
        // user removeInorder
        else if (command == "removeInorder"){
            int n;
            string next;
            getline(cin, next, ' ');
            cin >> n;
            tree.removeInOrder(n);
        }
        // user searchID or search NAME
        else if (command == "search") {
            // skip the space after the command
            cin.ignore(1, ' ');
            // if next character is a quote, we know it's a name
            if (cin.peek() == '"') {
                // skip quote
                cin.ignore();

                // read the name until the next quote
                string name;
                getline(cin, name, '"');

                // check name validation
                if (!verifyName(name)) {
                    cout << "unsuccessful" << endl;
                } else {
                    tree.searchName(name);
                }
            } else {
                // otherwise read ID
                string ID;
                getline(cin, ID);

                // input ID validation
                if (!verifyID(ID)) {
                    cout << "unsuccessful" << endl;
                } else {
                    tree.searchID(ID);
                }
            }
        }
        // wrong/misspelled command
        else {
            // ignoring 1000 characters to reset the command if it's unsuccessful
            cin.ignore(1000, '\n');
            cout << "unsuccessful" << endl;
        }
    }
}