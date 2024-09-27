SELECT DISTINCT u.name 
FROM user u
WHERE u.country = 'UK' OR u.country = 'Australia' 
ORDER BY u.name;