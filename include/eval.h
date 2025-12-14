#ifndef EVAL_H
#define EVAL_H

extern char *progmem;
void eval_line(const char *line);
void eval_prog(char **lines);

#endif