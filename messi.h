#define SIZE 100


enum parser_state {
	parsing_cmd,
	parsing_single_line,
	parsing_multi_line,
	parsing_retr,
	parser_error,
	exit_parser

};

enum cmd_state {
	r_found,
	reading
};

// enum single_line_state {
// 	r_found,
// 	reading
// }

enum multi_line_state {
	r1_found,
	n1_found,
	dot_found,
	r2_found,
	reading1
};

struct cmd_state_machine{
	enum cmd_state state;
};

struct single_line_state_machine {
	//enum single_line_state state;
		enum cmd_state state;

};

struct multi_line_state_machine {
	enum multi_line_state state;
};

struct retr_state_machine {
	enum multi_line_state state;
};

struct pop3_parser {
	char cmd[SIZE];
	enum parser_state state;
	struct cmd_state_machine cmd_sm;
	struct single_line_state_machine sl_sm;
	struct multi_line_state_machine ml_sm;
	struct retr_state_machine r_sm;
};



enum cmd_t {
	single_line,
	multi_line,
	retr,
	cmd_not_found
};


enum parser_state cmd_read( struct pop3_parser * p, char c );

enum parser_state single_line_read( struct pop3_parser * p, char c );

enum parser_state multi_line_read ( struct pop3_parser * p, char c );

enum parser_state retr_read ( struct pop3_parser * p, char c );