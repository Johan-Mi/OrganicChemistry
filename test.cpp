#include <string>
#include <iostream>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <random>
#include <chrono>

void writeWchar(wchar_t c) {
	mbstate_t state;
	memset(&state, 0, sizeof(state));
	char mbbuf[MB_CUR_MAX + 1];
	size_t nbytes = wcrtomb(mbbuf, c, &state);
	mbbuf[nbytes] = '\0';
	std::cout << mbbuf;
}

enum class Prefix {
	Met = 1, Et, Prop, But, Pent, Hex, Hept, Okt, Non, Dek
};

enum class Suffix {
	Alkan = 0, Alken, Alkyn, Alkohol, Syra
};

int main() {
	setlocale(LC_ALL, "");
	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

	while(true) {
		Prefix prefix;
		Suffix suffix;

		suffix = static_cast<Suffix>(rng() % 5);
		if(suffix == Suffix::Alken || suffix == Suffix::Alkyn)
			prefix = static_cast<Prefix>(rng() % 9 + 2);
		else
			prefix = static_cast<Prefix>(rng() % 10 + 1);

		if(rand() % 2) {
			int carbonCount = static_cast<int>(prefix);
			int hydrogenCount;
			int oxygenCount = 0;

			switch(suffix) {
			case Suffix::Alkan:
				hydrogenCount = 2 * carbonCount + 2;
				break;
			case Suffix::Alken:
				hydrogenCount = 2 * carbonCount;
				break;
			case Suffix::Alkyn:
				hydrogenCount = 2 * carbonCount - 2;
				break;
			case Suffix::Alkohol:
				hydrogenCount = 2 * carbonCount + 2;
				oxygenCount = 1;
				break;
			case Suffix::Syra:
				hydrogenCount = 2 * carbonCount;
				oxygenCount = 2;
				break;
			default:
				break;
			}
			
			std::string formula{};
			if(suffix == Suffix::Syra) {
				if(prefix != Prefix::Met) {
					formula = "CH3";
					if(prefix != Prefix::Et) {
						if(prefix != Prefix::Prop)
							formula += '(';
						formula += "CH2";
						if(prefix != Prefix::Prop) {
							formula += ')';
							formula += std::to_string(static_cast<int>(prefix) - 2);
						}
					}
				}
				formula += "COOH";
			} else {
				formula = "C";
				if(carbonCount != 1)
					formula += std::to_string(carbonCount);
				formula += 'H';
				formula	+= std::to_string(hydrogenCount - (suffix == Suffix::Alkohol));
				if(oxygenCount) {
					formula += 'O';
					if(suffix == Suffix::Alkohol)
						formula += 'H';
					else {
						if(oxygenCount != 1)
							formula += std::to_string(oxygenCount);
					}
				}
			}

			std::cout << "┌";
			for(int i = 0; i < formula.length(); i++)
				std::cout << "─";
			std::cout << "┐\n│" << formula << "│\n└";
			for(int i = 0; i < formula.length(); i++)
				std::cout << "─";
			std::cout << "┘\n\n";
		} else {
			int carbonCount = static_cast<int>(prefix);
			int picWidth;
			int picHeight = 5;

			switch(suffix) {
			case Suffix::Alkan:
				picWidth = 2 * carbonCount + 3;
				break;
			case Suffix::Alken:
				picWidth = 2 * carbonCount + 1;
				break;
			case Suffix::Alkyn:
				picWidth = 2 * carbonCount + 3;
				if(prefix == Prefix::Et)
					picHeight = 1;
				break;
			case Suffix::Alkohol:
				picWidth = 2 * carbonCount + 5;
				break;
			case Suffix::Syra:
				picWidth = 2 * carbonCount + 3;
				break;
			default:
				break;
			}

			auto pic = new wchar_t*[picHeight];
			for(int i = 0; i < picHeight; i++) {
				pic[i] = new wchar_t[picWidth];	
				for(int j = 0; j < picWidth; j++)
					pic[i][j] = L' ';
			}

			switch(suffix) {
			case Suffix::Alkan:
				pic[2][0] = L'H';
				pic[2][1] = L'─';
				pic[2][picWidth - 1] = L'H';
				for(int i = 0; i < 2 * carbonCount; i += 2) {
					pic[0][i + 2] = L'H';
					pic[1][i + 2] = L'│';
					pic[2][i + 2] = L'C';
					pic[3][i + 2] = L'│';
					pic[4][i + 2] = L'H';
					pic[2][i + 3] = L'─';
				}
				break;

			case Suffix::Alken:
				pic[0][0] = L'H';
				pic[1][0] = L'│';
				pic[2][0] = L'C';
				pic[3][0] = L'│';
				pic[4][0] = L'H';
				pic[2][1] = L'═';
				pic[0][2] = L'H';
				pic[1][2] = L'│';
				pic[2][2] = L'C';
				pic[2][picWidth - 2] = L'─';
				pic[2][picWidth - 1] = L'H';
				for(int i = 0; i < 2 * carbonCount - 4; i += 2) {
					pic[0][i + 4] = L'H';
					pic[1][i + 4] = L'│';
					pic[2][i + 4] = L'C';
					pic[3][i + 4] = L'│';
					pic[4][i + 4] = L'H';
					pic[2][i + 3] = L'─';
				}
				break;

			case Suffix::Alkyn: {
				int offset = prefix == Prefix::Et ? 0 : 2;
				pic[offset][0] = L'H';
				pic[offset][1] = L'─';
				pic[offset][2] = L'C';
				pic[offset][3] = L'≡';
				pic[offset][4] = L'C';
				pic[offset][picWidth - 2] = L'─';
				pic[offset][picWidth - 1] = L'H';
				for(int i = 0; i < 2 * carbonCount - 4; i += 2) {
					pic[0][i + 6] = L'H';
					pic[1][i + 6] = L'│';
					pic[2][i + 6] = L'C';
					pic[3][i + 6] = L'│';
					pic[4][i + 6] = L'H';
					pic[2][i + 5] = L'─';
				}
				break;
			}
			
			case Suffix::Alkohol:
				pic[2][0] = L'H';
				pic[2][1] = L'─';
				pic[2][picWidth - 3] = L'O';
				pic[2][picWidth - 2] = L'─';
				pic[2][picWidth - 1] = L'H';
				for(int i = 0; i < 2 * carbonCount; i += 2) {
					pic[0][i + 2] = L'H';
					pic[1][i + 2] = L'│';
					pic[2][i + 2] = L'C';
					pic[3][i + 2] = L'│';
					pic[4][i + 2] = L'H';
					pic[2][i + 3] = L'─';
				}
				break;

			case Suffix::Syra:
				pic[2][0] = L'H';
				pic[2][1] = L'─';
				pic[0][carbonCount * 2] = L'O';
				pic[1][carbonCount * 2] = L'‖';
				pic[2][carbonCount * 2] = L'C';
				pic[3][carbonCount * 2] = L'│';
				pic[4][carbonCount * 2] = L'O';
				pic[4][carbonCount * 2 + 1] = L'─';
				pic[4][carbonCount * 2 + 2] = L'H';
				for(int i = 0; i < 2 * carbonCount - 2; i += 2) {
					pic[0][i + 2] = L'H';
					pic[1][i + 2] = L'│';
					pic[2][i + 2] = L'C';
					pic[3][i + 2] = L'│';
					pic[4][i + 2] = L'H';
					pic[2][i + 3] = L'─';
				}
				break;

			default:
				break;
			}

			std::cout << "┌";
			for(int i = 0; i < picWidth; i++)
				std::cout << "─";
			std::cout << "┐\n";

			for(int i = 0; i < picHeight; i++) {
				std::cout << "│" << std::flush;
				for(int j = 0; j < picWidth; j++)
					writeWchar(pic[i][j]);
				std::cout << "│\n";
			}

			std::cout << "└";
			for(int i = 0; i < picWidth; i++)
				std::cout << "─";
			std::cout << "┘\n\n";

			for(int i = 0; i < picHeight; i++)
				delete[] pic[i];
			delete pic;
		}

		std::cout << "Gissning: ";
		std::string input;
		std::cin >> input;

		for(char& c : input) {
			if(c >= 'A' && c <= 'Z')
				c += 32;
		}

		if(input == "q")
			return 0;

		std::string answer;

		switch(prefix) {
		case Prefix::Met:
			answer = "met";
			break;
		case Prefix::Et:
			answer = "et";
			break;
		case Prefix::Prop:
			answer = "prop";
			break;
		case Prefix::But:
			answer = "but";
			break;
		case Prefix::Pent:
			answer = "pent";
			break;
		case Prefix::Hex:
			answer = "hex";
			break;
		case Prefix::Hept:
			answer = "hept";
			break;
		case Prefix::Okt:
			answer = "okt";
			break;
		case Prefix::Non:
			answer = "non";
			break;
		case Prefix::Dek:
			answer = "dek";
			break;
		default:
			break;
		}

		switch(suffix) {
		case Suffix::Alkan:
			answer += "an";
			break;
		case Suffix::Alken:
			answer += "en";
			break;
		case Suffix::Alkyn:
			answer += "yn";
			break;
		case Suffix::Alkohol:
			answer += "anol";
			break;
		case Suffix::Syra:
			answer += "ansyra";
			break;
		default:
			break;
		}

		if(input == answer)
			std::cout << "Rätt!\n\n";
		else
			std::cout << "Fel! Rätt svar är " << answer << ".\n\n";
	}

		

	return 0;
}
