<?php

//AXXX
class TMGMTBulkSelector_i18n_string extends TMGMTBulkSelector implements TMGMTBulkSelectorInterface {

  //AXXX
  public function searchItems(array $item_types, $translated_to, $not_exists_in_jobs, array $specific) {
    $ret = array();

    $query = db_select('i18n_string', 'i18n_s')->distinct();
    $item_id_expression = "concat(i18n_s.textgroup, ':', i18n_s.type, ':', i18n_s.objectid)";
    $item_id_field_alias = $query->addExpression($item_id_expression);

    uasort($item_types, array($this, '_sortItemTypes'));

    //AXXX
    $item_type_expression = '(CASE ';
    foreach ($item_types as $item_type) {
      $info = i18n_object_info($item_type);
      if ((isset($info['string translation']['type']) || isset($info['string translation']['textgroup'])) && !($item_type == 'field' || $item_type == 'field_instance')) {
        $conditions = array();
        if (isset($info['string translation']['type'])) {
          $conditions[] = "i18n_s.type = '{$info['string translation']['type']}'";
        }
        if (isset($info['string translation']['textgroup'])) {
          $conditions[] = "i18n_s.textgroup = '{$info['string translation']['textgroup']}'";
        }
        $item_type_expression .= "WHEN " . implode(' AND ', $conditions) . " THEN '$item_type' ";
      }
      elseif ($item_type == 'field' || $item_type == 'field_instance') {
        $bundles = array();
        foreach (entity_get_info() as $entity_info) {
          $bundles = array_merge($bundles, array_keys($entity_info['bundles']));
        }
        if (!empty($bundles)) {
          $item_type_expression .= "WHEN i18n_s.objectid "
              . ($item_type == 'field_instance' ? 'IN' : 'NOT IN')
              . " ('" . implode("', '", $bundles) . "') THEN '$item_type' ";
        }
      }
    }
    $item_type_expression .= "ELSE '' END)";
    $item_type_field_alias = $query->addExpression($item_type_expression);
    $query->havingCondition("$item_type_field_alias", $item_types, 'IN');

    if ($translated_to) {
      $target_language_table_alias = $query->leftJoin(
        'locales_target',
        NULL,
        "%alias.lid = i18n_s.lid AND %alias.language = :language",
        array(':language' => $translated_to)
      );
      $query->isNotNull("$target_language_table_alias.lid");
    }

    if ($not_exists_in_jobs) {
      $this->notExistsInJobs($query, 'i18n_string', $item_type_expression, $item_id_expression, $not_exists_in_jobs);//AXXX
    }

    foreach ($query->execute()->fetchAll() as $row) {
      $ret[] = new TMGMTBulkJobItem('i18n_string', $row->{$item_type_field_alias}, $row->{$item_id_field_alias});
    }
    return $ret;
  }

  //AXXX
  function _sortItemTypes($a, $b) {
    $weight = array();
    $info = array(
      'a' => i18n_object_info($a),
      'b' => i18n_object_info($b),
    );
    foreach ($info as $name => $type_info) {
      if (isset($type_info['string translation']['type']) && isset($type_info['string translation']['textgroup'])) {
        $weight[$name] = 2;
      }
      elseif (isset($type_info['string translation']['type'])) {
        $weight[$name] = 1;
      }
      elseif (isset($type_info['string translation']['textgroup'])) {
        $weight[$name] = 0;
      }
      elseif (${$name} == 'field') {
        $weight[$name] = -1;
      }
      else {
        $weight[$name] = -2;
      }
    }
    return $weight['b'] - $weight['a'];
  }
}
