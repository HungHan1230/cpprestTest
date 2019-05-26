#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <string>
#include <stdlib.h>

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
void Add()
{
    using concurrency::streams::basic_istream;
    using concurrency::streams::file_stream;

    file_stream<unsigned char>::open_istream("/home/hankvmcoin/testIPFSApi.txt").then([](pplx::task<basic_istream<unsigned char>> previousTask) {
        try
        {
            std::cout << "try\n"
                      << endl;
            auto fileStream = previousTask.get();
            // Get the content length, which is used to set the
            // Content-Length property
            fileStream.seek(0, std::ios::end);
            auto length = static_cast<size_t>(fileStream.tell());
            //fileStream.seek(0,0);

            //Make HTTP request with the file stream as the body
            http_client client(U("http://127.0.0.1:5001"));
            http_request request(methods::POST);
            request.headers().add("Content-Type", "multipart/form-data");
            request.set_request_uri("/api/v0/add");
            request.set_body(fileStream, length);

            return client.request(request)
                .then([fileStream](pplx::task<http_response> previousTask) {
                    fileStream.close();
                    std::wostringstream ss;
                    try
                    {
                        auto response = previousTask.get();
                        auto body = response.body();
                        ss << L"Server returned returned status code "
                           << response.status_code() << L"."
                           << std::endl;
                        std::wcout << ss.str();
                    }
                    catch (const http_exception &e)
                    {
                        ss << e.what() << std::endl;
                    }
                    std::wcout << ss.str();
                });
        }
        catch (exception ex)
        {
        }
    });

    // http_client client(U("http://127.0.0.1:5001"));

    // http_request request(methods::POST);

    // request.set_request_uri("/api/v0/add");

    // json::value postParameters = json::value::object();

    // postParameters["path"] = json::value::string("file@/home/hankvmcoin/testIPFSApi.txt");

    // request.set_body(postParameters);

    // pplx::task<http_response> responses = client.request(request);

    // cout << responses.get().to_string();

    printf("\nThe End...\n");
}
void AddStringtest()
{
    //string request_uri = "/api/v0/add";
    http_client client(U("http://127.0.0.1:5001/api/v0/add"));
    http_request request(methods::POST);
    // string headerstr = string("multipart/form-data; charset=utf-8; boundary=")+string("\"")+string("myboundary")+string("\"");

    // ostringstream s;
    // s << "multipart/form-data; charset=utf-8;"; // boundary=" << "\"" << "myboundary" << "\"";
    // string headerstr = s.str();

    string textBoundary = "--FORMBOUNDARY--";
    string textBody = "";
    textBody += "--" + textBoundary + "\r\n";
    textBody += "Content-Disposition:form-data;name=path\r\n";
    textBody += "\nasdfasdf\r\n";
    textBody += "--" + textBoundary + "--\r\n";

    request.headers().set_content_type("multipart/form-data;boundary=--FORMBOUNDARY--");
    request.headers().set_content_length(textBody.length());
    request.set_body(textBody);
    // cout << postParameters << endl;
    pplx::task<http_response> responses = client.request(request);
    cout << responses.get().to_string();
    printf("\nThe End...\n");
    cout << request.headers().content_type() << endl;
}

void Add2()
{
    printf("start Add2...\n");

    http_client httpClient("http://127.0.0.1:5001");

    // Simple GET request
    http_response httpResponse = httpClient.request(methods::GET).get();
    if (httpResponse.status_code() == status_codes::OK)
    {
        cout << httpResponse.to_string() << endl;
    }

    // Simple POST request
    string filePath = "/home/hankvmcoin/testIPFSApi.txt";
    Concurrency::streams::basic_istream<unsigned char> inputStream = file_stream<unsigned char>::open_istream(filePath).get();
    httpResponse = httpClient.request(methods::POST, "/api/v0/add", inputStream).get();
    if (httpResponse.status_code() == status_codes::OK)
    {
        cout << httpResponse.to_string() << endl;
    }
    printf("end Add2...\n");
}

void Add3()
{
    // Open stream to file.
    file_stream<unsigned char>::open_istream("./testIPFSApi.txt").then([](Concurrency::streams::basic_istream<unsigned char> fileStream) {
        // Make HTTP request with the file stream as the body.
        http_client client("http://127.0.0.1:5001");
        client.request(methods::POST, "/api/v0/add", fileStream).then([fileStream](http_response response) {
            fileStream.close();
            // Perform actions here to inspect the HTTP response…
            if (response.status_code() == status_codes::OK)
            {
                printf("say something...\n");
            }
        });
    });
}

int main(int argc, char *argv[])
{
    //g++ IPFS_class.cpp -o IPFS_class -lboost_system -lcrypto -lssl -lcpprest
    string str = "QmaaqrHyAQm7gALkRW8DcfGX3u8q9rWKnxEMmf7m9z515w";
    Get(str);
    AddStringtest(); //This add function is the only one that can be used
    //Add3();
    //sleep(5000);
}
