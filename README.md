# RatingPredection

* Works by taking in the large input file.
* Stores it into a hash table.
* Reads 0.0 ratings as it is being stored.
* Compares the user ratings to find the most likely user match it.
* Then predicts the movie rating using Pearsons correlation coefficient between 2 users.
* After filling the blank rating it will then write it into an output file in storing all users with the same UserID next to eachother.
