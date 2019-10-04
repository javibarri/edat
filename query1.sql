select extract(year from rental_date), count (rental_id)
  from  rental
  group by extract(year from rental_date);
