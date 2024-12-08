#include <iostream>
#include <string>
#include <gcrypt.h>


#ifndef NEED_LGBCRYPT_VERSION
#define NEED_LGBCRYPT_VERSION "1.11.0"
#endif


#ifndef ENCRYPTOR_H

#define ENCRYPTOR_H
namespace Encryptor {

	void init_lgcrypt() {
		
		if (!gcry_check_version(NEED_LGBCRYPT_VERSION)) {
			fprintf(
				stderr, "Versão do lgbcrypt necessária %s. (versão atual %s)",
				NEED_LGBCRYPT_VERSION, gcry_check_version(NULL));
			exit(1);
		}
		
		/* INICIALIANDO O LGBCRYPT SEM SECURE MEMORY */
		
		gcry_control(GCRYCTL_DISABLE_SECMEM, 0);
		
		gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
		
		/* INICIALIANDO O LGBCRYPT COM SECURE MEMORY */
		/*
		gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);
		
		gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);
		
		gcry_control(GCRYCTL_RESUME_SECMEM_WARN);
		
		gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);
		*/
	}

	void handle_gcrypt_error(gcry_error_t err) {
		if (err) {
			std::cerr << "Ocorreu um erro durante o processamento:  " << gcry_strerror(err) << std::endl;
			exit(1);
		}
	}

	void encrypt(std::string text, std::string key) {
		
		gcry_md_hd_t hd;
		gcry_md_open(&hd, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);
		
		gcry_md_write(hd, (unsigned char *)key.c_str(), key.length());
		
		unsigned char *hash = gcry_md_read(hd, GCRY_MD_SHA256);
		
		gcry_error_t err = gcry_control(GCRYCTL_SET_THREAD_CBS, 0);
		
		gcry_cipher_hd_t cipher_handle;
		
		err = gcry_cipher_open(&cipher_handle, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
		handle_gcrypt_error(err);
		
		err = gcry_cipher_setkey(cipher_handle, hash, 256);
		handle_gcrypt_error(err);
		
		unsigned char iv[16] {0};
		err = gcry_cipher_setiv(cipher_handle, iv, sizeof(iv));
		handle_gcrypt_error(err);
		
		unsigned char encrypted[1024];
		size_t length_text = text.length();
		err = gcry_cipher_encrypt(cipher_handle, encrypted, sizeof(encrypted),
								(unsigned char *)text.c_str(), length_text);
		handle_gcrypt_error(err);
		
		for (size_t i = 0; i < length_text; i++) {
			printf("%02x", encrypted[i]);
		}
	}

}
#endif