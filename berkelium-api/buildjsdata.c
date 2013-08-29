#include <stdio.h>
#include <stdlib.h>

void include(char* name, FILE* out)
{
	FILE* file = fopen(name, "r");

	if(file == NULL) {
		fprintf(stderr, "file '%s' not found!\n", name);
		exit(1);
	}

	fputc('"', out);
	fputs(name, out);
	fputs("\":\n\"", out);
	while(!feof(file)) {
		int c = fgetc(file);
		switch(c) {
			case -1:
				break;
			case 9:
				fputs("\\t", out);
				break;
			case 10:
				fputs("\\n\" +\n\"", out);
				break;
			case 13:
				break;
			case '"':
				fputs("\\\"", out);
				break;
			case '\\':
				fputs("\\\\", out);
				break;
			default:
				fputc(c, out);
				break;
		}
	}
	fputc('"', out);
	fclose(file);
}

int main(int argc, char** argv)
{
	int i;
	FILE* out;

	if(argc < 2) {
		fprintf(stderr, "syntax: '%s' <output> <input...>\n", argv[0]);
		exit(1);
	}
	out = fopen(argv[1], "w");
	if(out == NULL) {
		fprintf(stderr, "can not write into file '%s'!\n", argv[1]);
		exit(1);
	}
	fputs("var source={\n", out);
	for(i = 2; i < argc; i++) {
		if(i != 2) {
			fputc(',', out);
		}
		include(argv[i], out);
	}
	fputs("\n}\n", out);
	fclose(out);
	return 0;
}
