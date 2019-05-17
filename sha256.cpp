#include "user.h"

#define H0 0x6A09E667
#define H1 0xBB67AE85
#define H2 0x3C6EF372
#define H3 0xA54FF53A
#define H4 0x510E527F
#define H5 0x9B05688C
#define H6 0x1F83D9AB
#define H7 0x5BE0CD19

#define S(n, x) (((x) << (32 - (n))) | ((x) >> (n)))

unsigned int k[64] = {
	0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
	0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
	0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
	0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
	0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
	0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
	0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
	0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

unsigned int Wt[80];

unsigned int AA, BB, CC, DD, EE, FF, GG, HH;

/* Input to binary */
void m_convert_to_binary(const char *input, long long len, unsigned int *output)
{
	for (int i = 0; i < len; i++)
		for (int j = i * 8; j < i * 8 + 8; j++)
			output[j] = (input[i] >> (7 - j % 8)) & 1;
	return;
}

/* Num to binary */
void n_convert_to_binary(long long num, unsigned int *output)
{
	for (int i = 0; i < 64; i++)
		output[i] = (num >> (63 - i)) & 1;
}

/* Fill to 512-code */
void fill(long long len, unsigned int *output)
{
	if (len < 448)
	{
		output[len] = 1;
		for (int i = len + 1; i < 448; i++)
			output[i] = 0;
	}
	unsigned int num_len[64];
	n_convert_to_binary(len, num_len);
	for (int i = 448; i < 512; i++)
		output[i] = num_len[i - 448];
}

/* Calculate W[i] */
unsigned int cal_base(unsigned int *output, int group)
{
	unsigned int A = 0;
	for (int i = group * 32; i < group * 32 + 32; i++)
		A += (output[i] << (31 - (i - group * 32)));
	return A;
}

/* Generate SHA-256 value of input */
void SHA256_gen()
{
	for (int i = 16; i < 64; i++)
	{
		unsigned int S0 = S(7, Wt[i - 15]) ^ S(18, Wt[i - 15]) ^ (Wt[i - 15] >> 3);
		unsigned int S1 = S(17, Wt[i - 2]) ^ S(19, Wt[i - 2]) ^ (Wt[i - 2] >> 10);
		Wt[i] = Wt[i - 16] + S0 + Wt[i - 7] + S1;
	}
	unsigned int a = AA, b = BB, c = CC, d = DD, e = EE, f = FF, g = GG, h = HH;
	for (int i = 0; i < 64; i++)
	{
		unsigned int S0 = S(2, a) ^ S(13, a) ^ S(22, a);
		unsigned int MAJ = (a & b) ^ (a & c) ^ (b & c);
		unsigned int T2 = S0 + MAJ;
		unsigned int S1 = S(6, e) ^ S(11, e) ^ S(25, e);
		unsigned int ch = (e & f) ^ ((~e) & g);
		unsigned int T1 = h + S1 + ch + k[i] + Wt[i];
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;
	}
	AA += a, BB += b, CC += c, DD += d, EE += e, FF += f, GG += g, HH += h;
	return;
}

std::string sha256(std::string message)
{
	AA = H0, BB = H1, CC = H2, DD = H3, EE = H4, FF = H5, GG = H6, HH = H7;
	int msg_len = message.size();
	char *input = new char[msg_len];
	for (int i = 0; i < msg_len; i++)
		input[i] = message[i];
	unsigned int output[520];
	memset(output, 0, sizeof(output));
	long long len = message.length();
	m_convert_to_binary(input, len, output);
	fill(len * 8, output);
	memset(Wt, 0, sizeof(Wt));
	for (int i = 0; i < 16; i++)
		Wt[i] = cal_base(output, i);
	SHA256_gen();
	char *buffer = new char[65];
	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%08X%08X%08X%08X%08X%08X%08X%08X", AA, BB, CC, DD, EE, FF, GG, HH);
	std::string ans = buffer;
	delete[] input;
	delete[] buffer;
	//qDebug() << QString::fromStdString(ans) << "\n";
	//qDebug() << QString::fromStdString(ans) << "\n";
	return ans;
}