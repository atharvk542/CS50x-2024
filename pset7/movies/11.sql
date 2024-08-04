SELECT title FROM movies JOIN ratings on movies.id = ratings.movie_id WHERE id IN
(SELECT movie_id FROM stars WHERE person_id IN
(SELECT id FROM people WHERE name = 'Chadwick Boseman')) AND
(SELECT movie_id FROM ratings WHERE rating >= 9.0) ORDER BY rating DESC LIMIT 5;
