<?php

//AXXX
class TMGMTBulkSelector_entity extends TMGMTBulkSelector implements TMGMTBulkSelectorInterface {

  //AXXX
  public function searchItems(array $item_types, $translated_to, $not_exists_in_jobs, array $specific) {
    $ret = array();
    foreach ($item_types as $entity_type) {

      // Base query.
      $entity_info = entity_get_info($entity_type);
      $id_key = $entity_info['entity keys']['id'];
      $query = db_select($entity_info['base table'], 'e');
      $query->addField('e', $id_key);

      // Exclude entities with "und" source languages.
      $source_language_table_alias = $query->leftJoin(
        'entity_translation',
        NULL,
        "%alias.entity_type = :entity_type AND %alias.entity_id = e.$id_key AND %alias.source = ''",
        array(':entity_type' => $entity_type)
      );
      $query->condition("$source_language_table_alias.language", LANGUAGE_NONE, '<>');

      if ($translated_to) {
        $target_language_table_alias = $query->leftJoin(
          'entity_translation',
          NULL,
          "%alias.entity_type = :entity_type AND %alias.entity_id = e.$id_key AND %alias.language = :language",
          array(':entity_type' => $entity_type, ':language' => $translated_to)
        );
        $query->isNotNull("$target_language_table_alias.language");
        // Exclude entities with having source language same as the target
        // language we search for.
        $query->condition("$source_language_table_alias.language", $translated_to, '<>');
      }

      if ($not_exists_in_jobs) {
        $this->notExistsInJobs($query, 'entity', "'$entity_type'", "e.$id_key", $not_exists_in_jobs);
      }

      foreach ($query->execute()->fetchCol() as $entity_id) {
        $ret[] = new TMGMTBulkJobItem('entity', $entity_type, $entity_id);
      }
    }
    return $ret;
  }
}
