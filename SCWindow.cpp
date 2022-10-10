#include "SCWindow.h"

SC_Window::SC_Window(Point xy, int w, int h, const string& title)
    :Window{xy,w,h,title},
    next_button{Point{x_max()-170,0}, 100, 20, "Next Choice", cb_next},
    quit_button{Point{x_max()-75,0}, 80, 20, "Quit", cb_quit},

    process_button{Point{365,60}, 210, 20, "Read and process text file", cb_process},
    file_in{Point{200,60}, 150, 20, "Enter a text file to process:"},
    file_out{Point{200,85}, 150, 20, "Processed text file:"},

    solve_button{Point{625,140}, 200, 20, "Solve TOEFL question", cb_solve},
    TOEFL_in{Point{60,140}, 550, 20, ""},
    TOEFL_text{Point{60,130},"Enter a TOEFL question as <word> <answer> <choice1> <choice2> <choice3>:"},
    TOEFL_out{Point{60,165}, 770, 20, "TOEFL:"},

    welcome_message{Point{320,15},"Welcome to the Similarity Checker!"},
    menu{Point{160,30},300,20,Menu::horizontal,"Menu"},
    menu_button{Point{355,30}, 200, 20, "Similarity Checker Menu", cb_menu},

    intel_text{Point{60,225},"Test Similarity Checkers Intelligence"},
    intelligence_button{Point{430,240}, 150, 20, "Test Intelligence", cb_intelligence},
    intel_file{Point{260,240}, 150, 20, "Enter a TOEFL text file to test with:"},
    intel_out{Point{60,270}, 500, 20, "Score:"}
{
    attach(next_button);
    attach(quit_button);

    attach(process_button);
    attach(file_in);
    attach(file_out);
    file_out.put("No text file");

    attach(solve_button);
    TOEFL_text.set_font(Font::helvetica_bold_italic);
    attach(TOEFL_text);
    attach(TOEFL_in);
    attach(TOEFL_out);
    TOEFL_out.put("answer");

    welcome_message.set_font(Font::helvetica_bold_italic);
    attach(welcome_message);

    intel_text.set_font(Font::helvetica_bold_italic);
    attach(intel_text);
    attach(intelligence_button);
    attach(intel_file);
    attach(intel_out);
    intel_out.put("Program Intelligence Score");
    
    menu.attach(new Button{Point{0,0},0,0,"Read semantic dictionary from storage ",cb_read});
    menu.attach(new Button{Point{0,0},0,0,"Write semantic dictionary to storage",cb_write});
    attach(menu);
    menu.hide();
    attach(menu_button);
}

void SC_Window::read()
{
    ifstream ifs{DICTIONARY_FILE};
    //if failed
    if (!ifs) 
    {
        throw runtime_error("Could not open input file, ");
    }
    else
    {
        //extract from ifstream object into your dictionary object
        ifs >> d;
        ifs.close();
    }
}

void SC_Window::write()
{
    ofstream ofs{DICTIONARY_FILE};
    if (!ofs) 
    {
        throw runtime_error("Could not open output file, ");
    }
    //insert from the dictionary object into the ofstream object
    else 
    {
        ofs << d << endl;
        ofs.close();
    }
}

void SC_Window::process()
{
    string fileName = file_in.get_string();
    //extract file name
    //construct ifstream object and open the file
    ifstream ifs{fileName};
    if (!ifs) 
    {
        throw runtime_error("Can't open input file ");
    }
    else 
    {
        string text = "";
        string word;
        //loop to read every word in the file.  Concatenate each word onto text, separated by " "
        while (ifs >> word) text += word + " ";
        //pass text into getSentenceLists.
        vector<vector<string>> sentenceLists = getSentenceLists(text);
        //Create a double for loop to loop through each word of each sentenceList in sentenceLists and process the target word.
        for(vector<string> sentenceList : sentenceLists) 
            for(string targetWord : sentenceList) 
                d.processTargetWord(targetWord, sentenceList);
        file_out.put(fileName);      
    }
}

void SC_Window::solve()
{   
    string stream = TOEFL_in.get_string();
    vector<string>words;
    string word = "";
    for (size_t i = 0; i <= stream.size(); i++)
    {   
        if (stream[i] == ' ' || i == stream.size()) {
            words.push_back(word);
            word = "";
        }
        else 
            word += stream[i];
    }

    string wrd = words[0];
    string ans = words[1];
    string choice1 = words[2];
    string choice2 = words[3];
    string choice3 = words[4];
    try 
    {
        string s = d.mostSimilarWord(wrd, vector<string>{choice1, choice2, choice3});
        double similarity =  d.getSimilarity(wrd, s);
        if (s == ans) {
            string out = "Correct Answer | Word: " + wrd + " | Most similar: " + s + " | Similarity Index: " + to_string(similarity);
            TOEFL_out.put(out); 
        }
        else {
            string out = "Incorrect Answer | Word: " + wrd + " | Most similar: " + s + " | Similarity Index: " + to_string(similarity);
            TOEFL_out.put(out); 
        }
    }
    catch (runtime_error &e)
    {
        string out = e.what();
        TOEFL_out.put(out); 
    }
}

void SC_Window::testIntelligence()
{
    string testFile = intel_file.get_string();
    ifstream ifs{testFile};
    if (!ifs) {
        throw runtime_error("Could not open input file, ");
    }
    double totalQuestions, correctAns;  // keep track of total question and correct answers
    string word, answer, choice1, choice2, choice3;
    while (ifs >> word >> answer >> choice1 >> choice2 >> choice3) 
    {
        try 
        {
            string s = d.mostSimilarWord(word, vector<string>{choice1, choice2, choice3});
            if(s == answer) correctAns++;

            totalQuestions++;
        }
        catch (runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }
    double score = (correctAns / totalQuestions) * 100; // convert score to percentage
    stringstream ss;
    ss << fixed << setprecision(2) << score;
    string out = "Similarity Checker Intelligence Score: " + ss.str() + "% Correct";
    intel_out.put(out); 
}

void SC_Window::cb_next(Address, Address pw)
{
    reference_to<SC_Window>(pw).next();
}

void SC_Window::next()
{
    hide_menu();
    redraw();
}

void SC_Window::cb_quit(Address, Address pw)
{
    reference_to<SC_Window>(pw).quit();
}

void SC_Window::quit()
{
    hide();   
}

void SC_Window::cb_read(Address, Address pw)
{
    reference_to<SC_Window>(pw).read_pressed();
}

void SC_Window::cb_write(Address, Address pw)
{
    reference_to<SC_Window>(pw).write_pressed();
}

void SC_Window::cb_process(Address, Address pw)
{
    reference_to<SC_Window>(pw).process_pressed();
}

void SC_Window::cb_solve(Address, Address pw)
{
    reference_to<SC_Window>(pw).solve_pressed();
}

void SC_Window::cb_intelligence(Address, Address pw)
{
    reference_to<SC_Window>(pw).intelligence_pressed();
}

void SC_Window::cb_menu(Address, Address pw)
{
    reference_to<SC_Window>(pw).menu_pressed();
}