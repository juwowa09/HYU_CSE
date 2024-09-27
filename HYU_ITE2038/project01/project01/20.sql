SELECT u.country
FROM user u
GROUP BY u.country
ORDER BY count(*) DESC
LIMIT 1;