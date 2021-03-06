#include "day10.h"
#include "common/file_utils.h"

char Closer(char c)
{
	switch(c)
	{
		case '(': return ')';
		case '{': return '}';
		case '[': return ']';
		case '<': return '>';
	}

	fprintf(stderr, "No closing character for char: %c\n", c);
	exit(EXIT_FAILURE);
}

int ComputeScore(char c)
{
	switch(c)
	{
		case ')': return 3;
		case '}': return 1197;
		case ']': return 57;
		case '>': return 25137;

		case '(': return -1;
		case '[': return -2;
		case '{': return -3;
		case '<': return -4;
	}

	return 0;
}

void PrintCorruptionError(const size_t row, const size_t col, const size_t i_opener, const char read, const char * line)
{
	const char expected = Closer(line[i_opener]);
	printf("Error on line %ld, col %ld: Expected %c, found %c instead (badness %d)\n", row, col, expected, read, ComputeScore(read));
	printf("%s", line);
	for(size_t i=0; i<i_opener; ++i) printf(" ");
	printf("^");
	for(size_t i=i_opener; i<col-1; ++i) printf("~");
	printf("^\n\n");
}

void PrintWrongCharError(const size_t row, const size_t col, const char read, const char * line)
{
	printf("Error on line %ld, col %ld: Invalid character: %c\n", row, col, read);
	printf("%s", line);
	for(size_t i=1; i<col; ++i) printf("~");
	printf("^\n\n");
}

size_t ComputeLineAutocompletionScore(Stack stack, const char * line)
{
	size_t score = 0;
	while(stack.end != stack.begin)
	{
		size_t i = stack.end[-1];
		POP(stack);
		const char c = line[i];
		score *= 5;
		score -= ComputeScore(c);
	}
	return score;
}

Score ComputeLineScore(const char * line, const size_t line_id)
{
	Stack stack;
	NEW_VECTOR(stack);
	size_t col = 0;

	Score score;
	score.corruption = 0;
	score.autocompletion = 0;

	for(char const * c = line; *c != '\0' && *c!='\n'; ++c)
	{
		// PrintStack(stack);

		int char_score = ComputeScore(*c);

		if(char_score < 0) // Opener character
		{
			PUSH(stack, col);
		}
		else if(char_score > 0) // Closer caracter
		{
			size_t i_opener = stack.end[-1];
			POP(stack);
			char p = line[i_opener];

			if(*c != Closer(p)) // Corrupted line
			{
				// PrintCorruptionError(line_id, col, i_opener, *c, line);

				CLEAR(stack);
				score.corruption = char_score;
				return score;
			}
		}
		else {
			PrintWrongCharError(line_id, col, *c, line);
			exit(EXIT_FAILURE);
		}

		col += 1;
	}
	
	score.autocompletion = ComputeLineAutocompletionScore(stack, line);

	CLEAR(stack);
	return score;
}

DEFINE_QUICKSORT(QuickSort, long int)

Score Solve(const bool is_test)
{
	FILE * file = GetFile(is_test, 10);
	
	char * line = NULL;
	size_t len = 0;
	ssize_t read = 0;

	Score score;
	score.corruption = 0;
	score.autocompletion = 0;

	Vector autocompletions;
	NEW_VECTOR(autocompletions);

	size_t i = 0;
	while((read = getline(&line, &len, file)) != -1)
	{
		const Score line_score = ComputeLineScore(line, i);
		score.corruption += line_score.corruption;
		if(line_score.autocompletion != 0)
		{
			PUSH(autocompletions, line_score.autocompletion);
		}
		++i;
	}

	size_t vlen = SIZE(autocompletions);
	if(vlen % 2 == 0)
	{
		fprintf(stderr, "No middle item in autocompletions: Total is even! (%ld)", vlen);
		exit(EXIT_FAILURE);
	}


	QuickSort(autocompletions.begin, autocompletions.end);
	size_t middle = vlen / 2;
	score.autocompletion = autocompletions.begin[middle];

	free(line);
	fclose(file);
	CLEAR(autocompletions);

	return score;
}