#include "GUI.h"
#include "SemanticDictionary.h"
using namespace Graph_lib;

const string DICTIONARY_FILE = "SemanticDictionary.txt";

class SC_Window : Graph_lib::Window
{   
    public:
        SC_Window(Point xy, int w, int h, const string& title);

    private:
        SemanticDictionary d;

        Button next_button; 
        Button quit_button;

        Button process_button;
        In_box file_in;
        Out_box file_out;

        Button solve_button;
        In_box TOEFL_in;
        Text TOEFL_text;
        Out_box TOEFL_out;
    
        Text welcome_message;
        Menu menu;
        Button menu_button;

        Text intel_text;
        Button intelligence_button;
        In_box intel_file;
        Out_box intel_out;
        
        
        void read();
        void write();
        void process();
        void solve();
        void testIntelligence();
        void hide_menu() { menu.hide(); menu_button.show(); }
        
        // actions invoked by callbacks:
        void read_pressed() { read(); hide_menu(); }
        void write_pressed() { write(); hide_menu(); }
        void process_pressed() { process(); }    
        void solve_pressed() { solve(); }
        void intelligence_pressed() { testIntelligence(); }

        void menu_pressed() { menu_button.hide(); menu.show(); }
        void next();
        void quit();

        static void cb_read(Address, Address);
        static void cb_write(Address, Address);
        static void cb_process(Address, Address);
        static void cb_solve(Address, Address);
        static void cb_intelligence(Address, Address);
        static void cb_menu(Address, Address);
        static void cb_next(Address, Address);
        static void cb_quit(Address, Address);
};