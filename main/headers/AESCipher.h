#include <filesystem>
#include <cryptoPP/osrng.h>
#include <cryptoPP/modes.h>
#include <cryptoPP/files.h>
#include <cryptoPP/aes.h>
#include <cryptoPP/hex.h>
#include <cryptoPP/filters.h>

using namespace CryptoPP;

template <typename T, std::size_t N>
constexpr std::size_t FixedArraySize(T(&arr)[N]){ return N; }

class AESCipher {
public:
	AESCipher() {};
	~AESCipher() {};
	void AESEncrypt(std::string fileName, std::string newName) {
		std::cout << "Peforming AES encryption on " << fileName << std::endl;
		try
		{
			//Key & IV removed in Git commits. Enter your own below.


			const std::string extension = ".xml";
			const std::string sourceFile = fileName;
			const std::string destinationFile = std::filesystem::path(newName).replace_extension(extension).string();
			{
				CFB_Mode<AES>::Encryption encryption(reinterpret_cast<const CryptoPP::byte*>(key), FixedArraySize(key) - 1, IV);
				FileSource fsEnc(sourceFile.c_str(), true, new StreamTransformationFilter(encryption, new FileSink(destinationFile.c_str())));
			}
			std::filesystem::remove(sourceFile);
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	void AESDecrypt(std::string fileName, std::string newName) {
		std::cout << "Peforming AES decryption on " << fileName << std::endl;
		try
		{
			//Key & IV removed in Git commits. Enter your own below.


			const std::string extension = ".xml";
			const std::string sourceFile = fileName + ".xml";
			const std::string destinationFile = std::filesystem::path(fileName).replace_extension(extension).string();
			const std::string finalFile = std::filesystem::path(newName).replace_extension(extension).string();

			{
				CFB_Mode<AES>::Decryption decryption(reinterpret_cast<const CryptoPP::byte*>(key), FixedArraySize(key) - 1, IV);
				FileSource fsDec(destinationFile.c_str(), true, new StreamTransformationFilter(decryption, new FileSink(finalFile.c_str())));
			}
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	bool verifyHash(std::string fileName, Player &player) {
		std::cout << "Verifying hash" << std::endl;
		CryptoPP::SHA256 hash;
		std::string digest;
		std::filesystem::path cwd = std::filesystem::current_path() / "data" / "maps";
		cwd.append(fileName + ".tmx");
		std::cout << "hash loc: " << cwd << std::endl;
		FileSource fh(cwd.string().c_str(), true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
		if (digest != player.getMapHash(fileName)) {
			std::cout << "Invalid  hash : " << fileName << ".tmx has been modified" << std::endl;
			return false;
		}
		else
			return true;
		return false;
	}
};