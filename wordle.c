#include "wordlist.h"
enum chState charState[26], *wordState;
int len, chances;
const char *word, *meaning;
bool hard = false;
void init(void)
{
	srand(time(NULL));
	int line = rand() % wordCounts;
	word = wordlist[line].word;
	meaning = wordlist[line].meaning;
	len = strlen(word), chances = (len - 1) * 2;
	wordState = (enum chState *)malloc(sizeof(enum chState) * len);
	memset(wordState, 0, sizeof(wordState)); // state: unKnown
	memset(charState, 0, sizeof(charState)); // state: unKnown
}
bool checkCh(const char *_, char ch)
{
	for (int i = 0; i < len; i++)
		if (wordState[i] != right && _[i] == ch)
			return true;
	return false;
}
bool checkInput(const char *_)
{
	int _len = strlen(_);
	if (_len != len)
	{
		printf("Your word's length is %d not equals %d.\n", _len, len);
		return false;
	}
#ifdef HARD
	for (int i = 0; i < len; i++)
		if (wordState[i] == right && _[i] != word[i])
		{
			printf("The %d%s character must be '%c'.\n", i + 1, suffix[(i + 1) % 10], word[i]);
			return false;
		}
	for (int i = 0; i < len; i++)
		if (wordState[i] == exist)
			if (checkCh(_, word[i]) == false)
			{
				printf("Your word must contain '%c' (maybe more than one).\n", word[i]);
				return false;
			}
#endif
	return true;
}
void game(void)
{
	while (chances--)
	{
		printf("You have %d chance(s) now.\n", chances + 1);
		for (int i = 0; i < 26; i++)
			switch (charState[i])
			{
				case unKnown:   // black
					printf("%c ", i + 'a');
					break;
				case notExist:  // gray
					printf(gray "%c " cend, i + 'a');
					break;
				case right:     // green
					printf(green "%c " cend, i + 'a');
					break;
				case exist:     // yellow
					printf(yellow "%c " cend, i + 'a');
					break;
			}
		printf("\n");
		char *_ = (char *)malloc(sizeof(char) * (len));
		do {
			printf("Please enter a word of length %d. Press q to exit: ", len);
			scanf("%s", _);
			if (strcmp(_, "q") == 0 || strcmp(_, "Q") == 0)
				return;
		} while (checkInput(_) == false);

		enum chState *_State = (enum chState *)malloc(sizeof(enum chState) * len);
		memset(_State, 0, sizeof(_State)); // unknown
		int rightCount = 0;

		for (int i = 0; i < len; i++)
			if (_[i] == word[i])
			{
				_State[i] = wordState[i] = charState[_[i] - 'a'] = right; // green
				rightCount++;
			}
			else if (strchr(word, _[i]) == NULL)
				_State[i] = charState[_[i] - 'a'] = notExist; // gray
		for (int i = 0; i < len; i++) // yellow
			if (wordState[i] != right && wordState[i] != notExist)
				for (int j = 0; j < len; j++)
					if (word[i] == _[j] && _State[j] == unKnown)
					{
						wordState[i] = _State[j] = charState[word[i] - 'a'] = exist;
						break;
					}
		for (int i = 0; i < len; i++)
			switch (_State[i])
			{
				case right:
					printf(green "%c" cend, _[i]);
					break;
				case exist:
					printf(yellow "%c" cend, _[i]);
					break;
				default: // notExist
					printf(gray "%c" cend, _[i]);
					break;
			}
		printf("\n");
		if (rightCount == len)
		{
			printf("Congratulations!\n");
			ans(word, meaning, green);
			exit(0);
		}
	}
}
int main(void)
{
	init();
	game();
	printf("You failed...\n");
	ans(word, meaning, red);
	return 0;
}
