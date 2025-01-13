SELECT DISTINCT u.name
FROM youtuber y
JOIN country c on c.name = y.country
JOIN user u on y.youtuber_id = u.id
WHERE c.continent = 'Asia';
