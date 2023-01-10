# RatingPredection

* works by taking in the large input file.
* stores it into a hash table.
* reads 0.0 ratings as it is being stored.
* compares the user ratings to find the most likely user match it.
* then predicts the movie rating using Pearsons correlation coefficient between 2 users.
* after filling the blank rating it will then write it into an output file in storing all users with the same UserID next to eachother.
