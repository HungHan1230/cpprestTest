#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <string>

using namespace std;
using namespace utility;              // Common utilities like string conversions
using namespace web;                  // Common features like URIs.
using namespace web::http;            // Common HTTP functionality
using namespace web::http::client;    // HTTP client features
using namespace concurrency::streams; // Asynchronous streams

void Get(string str)
{
    string request_uri = "/api/v0/object/get?arg=" + str + "&encoding=json";

    http_client client(U("http://127.0.0.1:5001"));

    http_request request(methods::GET);

    // request.set_request_uri("/api/v0/object/get?arg=QmaaqrHyAQm7gALkRW8DcfGX3u8q9rWKnxEMmf7m9z515w&encoding=json");

    request.set_request_uri(request_uri);

    pplx::task<http_response> responses = client.request(request);

    pplx::task<web::json::value> jvalue = responses.get().extract_json();

    cout << jvalue.get();

    printf("\nThe End...\n");
}



int main(int argc, char *argv[])
{
    //g++ IPFS_class.cpp -o IPFS_class -lboost_system -lcrypto -lssl -lcpprest
    string str = "QmaaqrHyAQm7gALkRW8DcfGX3u8q9rWKnxEMmf7m9z515w";
    Get(str);   
}
