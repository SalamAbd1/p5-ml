#include <iostream>
#include "csvstream.hpp"
#include "float.h"
#include <set>
#include <cmath>
using namespace std;


// EFFECTS: Return a set of unique whitespace delimited words.
set<string> unique_words(const string &str) {
    istringstream source(str);
    set<string> words;
    string word;
    while (source >> word) {
        words.insert(word);
    }
    return words;
}

/* Reads in the data from the .csv file
 * Records the following info: 
 * - The total number of posts in the entire training set.
 * - The number of unique words in the entire training set. (The vocabulary size.)
 * - For each word w, the number of posts in the entire training set that contain w.
 * - For each label C, the number of posts with that label.
 * - For each label C, and word w, the number of posts with label C hat contain w 
*/

class Classifier {

public:
    Classifier() {}

    Classifier(csvstream &csv_in, bool debug) {
        map<string, string> post;
        while(csv_in >> post) {
            string label = post["tag"];
            string words = post["content"];
            ++labels[label];
            set<string> unique = unique_words(words);
            for (string word : unique) { 
                ++vocab[word]; 
                ++labels_words[{label, word}];
            }
            if (debug) printTrainingData(post);
            post.clear();
            numPosts++;
        }
    }

    void printTrainingData(map<string, string> &post) {
        cout << "  label = " << post["tag"]
             << ", content = " << post["content"] << endl;
    }

    void printClasses() {
        cout << "classes:" << endl;
        for (auto label : labels) {
            cout << "  " << label.first << ", " << label.second 
                 << " examples, log-prior = " << log_prior(label.first) << endl;
        }
    }

    void printClassifierParameters() {
        cout << "classifier parameters:" << endl;
        for (auto label_word : labels_words) {
            cout << "  " << label_word.first.first << ":" << label_word.first.second 
                 << ", count = " << label_word.second << ", log-likelihood = "
                 << log_likelihood(label_word.first.first, label_word.first.second)
                 << endl;
        }
        cout << endl;
    }
    
    // EFFECTS: Returns the total number of posts in the entire training set.
    int getNumPosts() {
        return numPosts;
    }
    
    // EFFECTS: Returns the number of unique words in the entire training set.
    int getVocabSize() {
        return vocab.size();
    }

     // EFFECTS: Returns the log-probability score for a label on a certain post.
    double log_probability(set<string> &post, const string &label) {
        double logProb = log_prior(label); 
        for (auto word : post) { logProb += log_likelihood(label, word); }
        return logProb;
    }

    // EFFECTS: Returns the log-likelihood score for a word 
    //          as a reflection of how common it is in posts with a certain label.
    double log_likelihood(const string &label, const string &word) {
        if (vocab.find(word) == vocab.end()) {
            return log(1.0 / getNumPosts());
        }
        else if (labels_words[{label, word}] == 0) {
            return log(vocab[word] / static_cast<double>(getNumPosts())); 
        }
        return log(labels_words[{label, word}] / static_cast<double>(labels[label]));
    }

    // EFFECTS: Returns the log-prior score for a label 
    //          as a reflection of how common it is.
    double log_prior(const string &label) {
        return log(labels[label] / static_cast<double>(getNumPosts()));
    }
    
    // EFFECTS: predicts label given post
    string predictLabel(set<string> &post) {
        double max_prob = -DBL_MAX;
        string prediction;
        for (auto label : labels) {
            if (log_probability(post, label.first) > max_prob) { 
                max_prob = log_probability(post, label.first);
                prediction = label.first;
            }
            else if (log_probability(post, label.first) > max_prob) {
                if (label.first < prediction) prediction = label.first;
            }
        }
        return prediction;
    }
    
private:
    int numPosts = 0;
    map<string, int> vocab; // word, number of posts with word
    map<string, int> labels; // label, number of posts with label
    map<pair<string, string>, int> labels_words; // num posts with label C and word W
};


int main(int argc, char* argv[]) {
    cout.precision(3); 
    string debug = "--debug";
    if ((argc != 4 && argc != 3) || (argc == 4 && argv[3] != debug)) { 
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1; 
    }

    try { csvstream trainStream(argv[1]); }

    catch (csvstream_exception &failToOpen) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    try { csvstream testStream(argv[2]); }

    catch (csvstream_exception &failToOpen) {
        cout << "Error opening file: " << argv[2] << endl;
    }

    bool debugged = argc == 4;

    if (debugged) cout << "training data:" << endl;

    // streams
    csvstream trainStream(argv[1]);
    csvstream testStream(argv[2]);

    // classifier and data
    Classifier Genius(trainStream, debugged);
    map<string, string> test_post;
    int numCorrect = 0;
    
    // output
    cout << "trained on " << Genius.getNumPosts() << " examples" << endl;
    if (debugged) { 
        cout << "vocabulary size = " << Genius.getVocabSize() << endl << endl;
        Genius.printClasses();
        Genius.printClassifierParameters();        
    }
    else { cout << endl; }
    cout << "test data:" << endl;

    int numTest;
    
    for(numTest = 0; testStream >> test_post; numTest++) {
        set<string> unique = unique_words(test_post["content"]);
        string prediction = Genius.predictLabel(unique); 
        string correct = test_post["tag"];
        cout << "  correct = " << correct << ", predicted = "
             << prediction << ", log-probability score = "
             << Genius.log_probability(unique, prediction) << endl
             << "  content = " << test_post["content"] << endl;
        if (prediction == correct) { numCorrect++; }
        cout << endl;
        test_post.clear();
    }

    cout << "performance: " << numCorrect << " / " << numTest
         << " posts predicted correctly" << endl;
}
