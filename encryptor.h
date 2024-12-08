#include <gcrypt.h>

namespace Encryptor
{
	void init_lgcrypt();
	void handle_gcrypt_error(gcry_error_t err);
	void encrypt(std::string text, std::string key);
}