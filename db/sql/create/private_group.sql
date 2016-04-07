DROP TABLE private_group;

CREATE TABLE private_group(
	"vgroup_id" INTEGER PRIMARY KEY NOT NULL,
	"summ" REAL NOT NULL DEFAULT 0.
);

ALTER TABLE private_group
	ADD CONSTRAINT vgroup_id_fk
		FOREIGN KEY (vgroup_id) REFERENCES vgroup (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;