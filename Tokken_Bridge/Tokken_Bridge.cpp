// Tokken_Bridge.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>


struct VpnServer {
    std::string hostName;
    std::string ip;
    long score;
    int ping;
    long long speed;
    std::string countryLong;
    std::string countryShort;
    int numVpnSessions;
    long long uptime;
    int totalUsers;
    unsigned long long totalTraffic;
    std::string logType;
    std::string operatorName;
    std::string message;
    std::string openVpnConfigData;
};

std::vector<VpnServer> parseCsv(const std::string& csvData)
{
    std::istringstream dataStream(csvData);
    std::string line;
    std::vector<VpnServer> vpnServers;

    // Skipping the first line starting with an asterisk
    std::getline(dataStream, line);
    if (!line.empty() && line.front() == '*') {
        std::getline(dataStream, line); // Skip the header line
    }

    int lineNumber = 2; // Start at line 2 (line 1 is skipped above)

    while (std::getline(dataStream, line))
    {
        std::istringstream lineStream(line);
        std::string token;
        std::vector<std::string> tokens;

        // Ignore lines starting with an asterisk
        if (!line.empty() && line.front() == '*') {
            lineNumber++;
            continue;
        }

        // Splitting the line by comma delimiter
        while (std::getline(lineStream, token, ','))
        {
            tokens.push_back(token);
        }

        // Check if there are the correct number of tokens (15)
        if (tokens.size() != 15) {
            std::cout << "Error parsing line " << lineNumber << ": expected 15 tokens, got " << tokens.size() << std::endl;
            lineNumber++;
            continue; // Skip this line
        }

        VpnServer server;

        try {
            server.hostName = tokens[0];
            server.ip = tokens[1];
            server.ping = std::stoi(tokens[3]);
            server.speed = std::stoll(tokens[4]);
            server.countryLong = tokens[5];
            server.countryShort = tokens[6];
            server.numVpnSessions = std::stoi(tokens[7]);
            server.uptime = std::stoll(tokens[8]);
            server.totalUsers = std::stoi(tokens[9]);
            server.totalTraffic = std::stoll(tokens[10]);
            server.logType = tokens[11];
            server.operatorName = tokens[12];
            server.message = tokens[13];
            server.openVpnConfigData = tokens[14];

        }

        catch (const std::out_of_range& e) {
            std::cerr << "Error creating VpnServer for line " << lineNumber << ": out_of_range exception: " << e.what() << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error creating VpnServer for line " << lineNumber << ": invalid_argument exception: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "Error creating VpnServer for line " << lineNumber << ": unknown exception" << std::endl;
        }

        try {
            server.score = std::stol(tokens[2]);
        }
        catch (...) {
            std::cerr << "Error converting score on line " << lineNumber << ": " << tokens[2] << std::endl;
        }



        vpnServers.push_back(server);
        lineNumber++;
    }

    return vpnServers;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

std::string getDataFromUrl(const std::string& url)
{
    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    return readBuffer;
}


std::vector<VpnServer> filterAndSortServers(const std::vector<VpnServer>& serverList)
{
    // Filter non-Japanese servers
    std::vector<VpnServer> filteredServers;
    std::copy_if(serverList.begin(), serverList.end(), std::back_inserter(filteredServers),
        [](const VpnServer& server) { return server.countryShort == "JP"; });

    // Sort servers by vpn connection (load)
    std::sort(filteredServers.begin(), filteredServers.end(),
        [](const VpnServer& a, const VpnServer& b) { return a.numVpnSessions < b.numVpnSessions; });

    return filteredServers;
}

int main()
{
   
    // Fetch the list of available VPN servers
    std::string vpnServerData = getDataFromUrl("https://www.vpngate.net/api/iphone/");

    // Parse the CSV data and store it in a vector of VpnServer objects
    std::vector<VpnServer> vpnServerList = parseCsv(vpnServerData);

    // Filter and sort servers
    std::vector<VpnServer> filteredAndSortedServers = filterAndSortServers(vpnServerList);
    for (const auto& server : filteredAndSortedServers)
    {
        std::cout << "Hostname: " << server.hostName << " | IP: " << server.ip
            << " | Speed: " << server.speed << " | Country: " << server.countryLong << " | Load : " << server.numVpnSessions << std::endl;
    }
    std::cin.get();
    return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
