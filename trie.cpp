#include <iostream>
#include <string>
#define false 0
#define true 1
#define MAX_WORD_SIZE   5000
#define MAX_MEANING_SIZE 10000
using namespace std;


struct trieNode
{
    struct trieNode* children[26];
    bool isEndOfWord;
    string data;
};

struct trieNode* getNode(void)
{
    struct trieNode *ptr =  new trieNode;

    ptr->isEndOfWord = false;

    for (int i = 0; i < 26; i++)
        ptr->children[i] = NULL;

    return ptr;
}

int letternumber(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A';
    }

    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }
    return -1;
}
int insert(struct trieNode *root, string word, string meaning)
{
    struct trieNode *rootptr = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letternumber(word[i]);

        if (!rootptr->children[index])
            rootptr->children[index] = getNode();

        rootptr = rootptr->children[index];
    }

    rootptr->isEndOfWord = true;
    rootptr->data = meaning;

    return true;
}

int readWordsFromFile(struct trieNode *root, const char* filename)
{

    FILE *file = fopen(filename, "r");

    if (!file)
    {
        cout<<"could not find/open file"<<filename;
        return false;
    }

    char word[MAX_WORD_SIZE];
    char meaning[MAX_MEANING_SIZE];

    int count = 0;

    while (fscanf(file,"%s %[^\n]",word,meaning) > 1)
    {
        if (!insert(root, word, meaning))
        {
            fclose(file);
            return false;
        }

        else
        {
            count++;
        }
    }

    fclose(file);
    cout << "Welcome to our Dictionary. Our Dictionary right now contains " << count << " words.\n";
    cout<<"\n\t\t\tTOTAL NO OF WORDS IN OUR DICTIONARY :"<<count<<endl;;
    return true;
}

bool search(struct trieNode *root, string word)
{
    struct trieNode *rootptr = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letternumber(word[i]);

        if (!rootptr->children[index])
            return false;

        rootptr = rootptr->children[index];
    }
    if (rootptr != NULL && rootptr->isEndOfWord)
    {
        cout << "The meaning of the word is: \n";
        cout << rootptr->data;
    }
    return true;
}

bool deleteWord(struct trieNode *root, string word)
{
    struct trieNode *rootptr = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letternumber(word[i]);

        if (!rootptr->children[index])
            return false;

        rootptr = rootptr->children[index];
    }

    if (rootptr != NULL && rootptr->isEndOfWord)  {
        rootptr->isEndOfWord = false;
        return true;
    }
}

void alphabeticalOrder(struct trieNode* root, char allWords[], int index)
{
    struct trieNode* rootptr = root;

    if(rootptr != NULL && rootptr->isEndOfWord == true) {
        for(int j = 0; j < index; j++)  {
            cout << allWords[j];
            }

        cout << ": ";
        cout << rootptr->data << "\n";
    }

    for(int i = 0; i < 26; i++)  {
        if(rootptr->children[i] != NULL) {
            allWords[index] = 'a' + i;
            alphabeticalOrder(rootptr->children[i], allWords, index + 1);
        }
    }
}

void print_prefix_search(struct trieNode* root, char allWords[], int index, string prefix)
{
    struct trieNode* rootptr = root;

    if(rootptr != NULL && rootptr->isEndOfWord == true) {
        cout << prefix;

        for(int j = 0; j < index; j++)  {
            cout << allWords[j];
            }

        cout << ": ";
        cout << rootptr->data << "\n";
    }

    for(int i = 0; i < 26; i++)  {
        if(rootptr->children[i] != NULL) {
            allWords[index] = 'a' + i;
            print_prefix_search(rootptr->children[i], allWords, index + 1, prefix);
        }
    }
}

void prefix_search(struct trieNode* root, string prefix)
{
    struct trieNode* rootptr = root;

    for(int i = 0; i < prefix.length(); i++)    {
        int index = letternumber(prefix[i]);
        rootptr = rootptr->children[index];
    }

    char allWords[50];
    print_prefix_search(rootptr, allWords, 0, prefix);
}

bool searchSuggestedWord(struct trieNode* root, string word, int count)
{
     struct trieNode *rootptr= root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letternumber(word[i]);

        if (!rootptr->children[index])
            return false;

        rootptr = rootptr->children[index];
    }

    if (rootptr != NULL && rootptr->isEndOfWord)  {
        if(count == 0)  {
            cout << "Suggested words are: ";
        }
        cout << word << "  ";
    }

    return true;
}

/*bool suggestedWords(struct trieNode* root, string word, int number)
{
    int count = 0;

    for(int i = 0; i < word.length(); i++)  {
        string temp = word;

        for(int j = 0; j < 26; j++)  {
            if(count < number && j != letternumber(word[word.length() - i - 1]))  {
                temp[word.length() - i - 1] = j + 97;

                if(searchSuggestedWord(root, temp, count)) {
                    count++;
                }
            }
        }
    }

    if(count == 0)  {
            cout << "\nSorry we couldn't find any words related to " << word << " in our Dictionary.\n";
        return false;
    }
    else if (count < number)    {
        cout << "\nWe could only find " << count << " words related to " << word << " in our Dictionary.\n";
        return true;
    }
    else    {
        return true;
    }
}*/

int main()  {
    struct trieNode *root = getNode();

    readWordsFromFile(root, "words.txt");

    string command;
    do
    {
        cout << "\nWhat would you like to do?\n";
        cout << "1. Insert a word.\n";
        cout << "2. Search a word.\n";
        cout << "3. Delete a word.\n";
        cout << "4. Print dictionary in alphabetical order.\n";
        cout << "5. Prefix search.\n";
        //cout << "6. Show suggested words.\n";
        cout << "\n";
        fflush(stdin);

        int call;
        cin >> call;
        cout << "\n";

        string word;
        string meaning;

        switch(call)    {

            case 1: cout << "Enter the word you would like to insert: ";
                    cin >> word;
                    cout << "Enter its meaning: ";
                    cin >> meaning;

                    if(insert(root, word, meaning)) {
                        cout << word << " has been added to the Dictionary.\n";
                    }

                    break;

            case 2: cout << "Enter the word you would like to search: ";
                    cin >> word;

                    if(!search(root, word))  {
                        cout << "Sorry, the word you searched for doesn't exist. Would you like to add it to the Dictionary.(Yes/No) ";

                        string add;
                        cin >> add;

                        if(add == "yes" || add == "Yes" || command == "YES")  {
                            cout << "Enter its meaning: ";
                            cin >> meaning;

                            if(insert(root, word, meaning)) {
                            cout << word << " has been added to the Dictionary.\n";
                            }
                        }
                    }

                    break;

            case 3: cout << "Which word would you like to delete? ";
                    cin >> word;

                    if(deleteWord(root, word))  {
                        cout << word << " has been successfully deleted from the Dictionary.\n";
                    }

                    else    {
                        cout << "No such word exists in the Dictionary.\n";
                    }

                    break;

            case 4: char allWords[50];
                    alphabeticalOrder(root, allWords, 0);
                    cout << "\n";

                    break;

            case 5: cout << "Enter the word you would like to use as a prefix: ";
                    cin >> word;
                    cout << "\n";

                    prefix_search(root, word);

                    break;

            /*case 6: cout << "Enter the word for whose suggested words you want to see: ";
                    cin >> word;
                    int number;
                    cout << "How many suggested words do you want to see: ";
                    cin >> number;

                    suggestedWords(root, word, number);

                    break;*/

            default: cout << "Enter a valid entry.";
        }
    fflush(stdin);
        cout << "\n\nWould you like to continue?(Yes/N0) ";
        cin >> command;
    }
    while(command == "yes" || command == "Yes" || command == "YES");

    cout << "Thanks for using our Dictionary.\n";
    return 0;
}
