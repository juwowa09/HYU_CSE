SELECT count(*) notExplorer
FROM PlayableCharacter p
WHERE NOT p.class = 'Explorer' ;