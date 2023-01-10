#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <sstream>

#define code Project.V3

using namespace std;
using namespace std::chrono;

// A structure to represent a rating given by a user to a movie
struct Rating {
    int userId;
    int movieId;
    double rating;
};

// A structure to represent a user and their ratings
struct User {
    int userId;
    unordered_map<int, double> ratings;
};

// A structure to represent a movie and the ratings it has received
struct Movie {
    int movieId;
    unordered_map<int, double> ratings;
};

// A utility function to calculate the Pearson correlation coefficient between two users
double calculateSimilarity(const User& u1, const User& u2) {
    int n = 0;
    double sum1 = 0.0, sum2 = 0.0, sum1Sq = 0.0, sum2Sq = 0.0, pSum = 0.0;

    for (const auto& [movieId, rating] : u1.ratings) {
        if (auto it = u2.ratings.find(movieId); it != u2.ratings.end()) {
            ++n;
            sum1 += rating;
            sum2 += it->second;
            sum1Sq += rating * rating;
            sum2Sq += it->second * it->second;
            pSum += rating * it->second;
        }
    }

    if (n == 0) {
        return 0;
    }

    double num = pSum - (sum1 * sum2 / n);
    double den = sqrt((sum1Sq - (sum1 * sum1) / n) * (sum2Sq - (sum2 * sum2) / n));

    if (den == 0) {
        return 0;
    }

    return num / den;
}

// A function to predict the rating of a movie for a given user using UBCF
double predictRating(const User& user, const Movie& movie) {
    vector<pair<double, int>> sims;

    for (const auto& [userId, rating] : movie.ratings) {
        if (userId != user.userId) {
            User otherUser;
            otherUser.userId = userId;
            otherUser.ratings[movie.movieId] = rating;
            double sim = calculateSimilarity(user, otherUser);
            sims.emplace_back(sim, rating);
        }
    }

    // Sort the similarities in decreasing order
    sort(sims.rbegin(), sims.rend());

    // Take the top k most similar users (where k is a user-defined parameter)
    int k = 3;
    double sum = 0.0;
    int count = 0;
    for (int i = 0; i < k && i < sims.size(); ++i) {

        sum += sims[i].first * sims[i].second;
        count++;
    }

    // If no similar users were found, return the rating for the movie
    if (count == 0) {
        double mean = 0.0;
        for (const auto& [_, rating] : movie.ratings) {
            mean += rating;
        }
        return mean / movie.ratings.size();
    }

    // Return the predicted rating for the movie
    return sum / count;
}

int main() {
    auto start = high_resolution_clock::now();
    // Load the ratings data from a file or database

    unordered_map<int, User> users;
    unordered_map<int, Movie> movies;

    ifstream file("train.csv");
    ofstream outputFile("output.csv");
    outputFile << "UserID,ItemID,rating" << std::endl;

    if (file.is_open()) {
        string line;
        getline(file, line); // skip the first line (header)
        while (getline(file, line)) {
            Rating rating;
            stringstream ss(line);
            char separator;
            ss >> rating.userId >> separator >> rating.movieId >> separator >> rating.rating;
            users[rating.userId].userId = rating.userId;
            users[rating.userId].ratings[rating.movieId] = rating.rating;
            movies[rating.movieId].movieId = rating.movieId;
            movies[rating.movieId].ratings[rating.userId] = rating.rating;
        }
    }

    file.close();

    // Process all of the ratings and write the predicted ratings to the output file
    for (const auto& [_, user] : users) {
        for (const auto& [movieId, rating] : user.ratings) {
            if (rating == 0.0) {
                double predictedRating = predictRating(user, movies[movieId]);
                outputFile << user.userId << ',' << movieId << ',' << predictedRating << std::endl;
            } else {
                outputFile << user.userId << ',' << movieId << ',' << rating << std::endl;
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    double seconds = duration.count() / 1000000.0;
    cout << "Time taken: " << duration.count() << " microseconds" << endl;
    cout << "Time Taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count() << " milliseconds" << std::endl;
    cout << "Time taken: " << seconds << " seconds" << endl;
    cout << "Time taken: " << seconds / 60.0 << " minutes" << endl;
    return 0;
}
