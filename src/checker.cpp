#define _CRT_SECURE_NO_WARNINGS
#include <curl/curl.h>
#include <iostream>
#include <windows.h>
#include <json/json.h>
#include <WinSock2.h>
#include <vector>


namespace
{
	std::size_t cb(
		const char* in,
		std::size_t taille,
		std::size_t num,
		std::string* out)
	{
		const std::size_t octet(taille * num);
		out->append(in, octet);
		return octet;
	}
}


void tokenverif()
{
	std::string const api = "https://discord.com/api/v6/users/@me";
	std::string token;

	CURL* init = curl_easy_init();

	std::string rep;
	long httpcode = 0;
	std::unique_ptr<std::string> donnee(new std::string());

	curl_easy_setopt(init, CURLOPT_URL, api.c_str());
	curl_easy_setopt(init, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
	curl_easy_setopt(init, CURLOPT_TIMEOUT, 10);
	curl_easy_setopt(init, CURLOPT_FOLLOWLOCATION, 1L);

	curl_easy_setopt(init, CURLOPT_WRITEFUNCTION, cb);
	
	struct curl_slist* header = NULL;
	header = curl_slist_append(header, "Authorization: VotreToken");
	

	curl_easy_setopt(init, CURLOPT_WRITEDATA, donnee.get());
	curl_easy_setopt(init, CURLOPT_HTTPHEADER, header);

	curl_easy_perform(init);
	curl_easy_getinfo(init, CURLINFO_RESPONSE_CODE, &httpcode);
	curl_easy_cleanup(init);
	std::cout << "HTTPCODE: " << httpcode << "\n";
	std::cin.ignore();

	if (httpcode == 200)
	{
		std::cout << "Reussis ! (1) \n";

		Json::Value jsondonnee;
		Json::Reader jsonlire;

		if (jsonlire.parse(*donnee.get(), jsondonnee))
		{
			std::cout << "Reussis ! (2) \n" << std::endl;
			std::cout << jsondonnee.toStyledString() << std::endl;
			std::cin.ignore();
		}
		else
		{
			std::cout << "On ne peut pas lire les donnes en json" << std::endl;
			std::cout << *donnee.get() << std::endl;
			std::cin.ignore();
		}
	}
	else
	{
		std::cout << "\nNous n'avons pas acces au site [" << api << "]" << " ,voici le code d'erreur [" << httpcode << "]\n";
		if (httpcode == 0)
		{
			std::cout << "Le site n'existe pas, cause timeout de 10 secondes " << "(solution verifier l'url)";
		}
		else if (httpcode == 401)
		{
			std::cout << "Vous n'etes pas autoriser a acceder au site " << "(solution verifier votre token)";
		}
		std::cin.ignore();
	}

}

